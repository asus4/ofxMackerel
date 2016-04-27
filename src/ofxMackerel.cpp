//
//  ofxMackerel.cpp
//
//  Copyright 2015 asus4
//
//

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>

#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>

#include <dispatch/dispatch.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include <future>

#include "ofxMackerel.hpp"

const std::string mackerel_origin = "https://mackerel.io";

ofxMackerel::Mackerel() {
    timeout = 10;  // in sec
    interval = 60;  // in sec
}

void ofxMackerel::setup(const std::string _apiKey, const std::string _id) {
    apiKey = _apiKey;
    if (_id == "") {
        id = loadId();
    } else {
        id = _id;
    }

    // Setup SSL
    // http://stackoverflow.com/questions/18315472/https-request-in-c-using-poco
    using Poco::Net::SSLManager;
    using Poco::Net::AcceptCertificateHandler;
    using Poco::Net::Context;
    Poco::Net::initializeSSL();
    SSLManager::InvalidCertificateHandlerPtr ptrHandler(new AcceptCertificateHandler(false));
    Context::Ptr ptrContext (new Context(Context::CLIENT_USE, ""));
    SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);
}

void ofxMackerel::queueMetrics(const std::string metrics, const float value) {
    metricsQueue[metrics] = value;

    // check 60 sec
    if (lastPostTime + interval < ofGetUnixTime()) {
        sendAllMetrics();
    }
}

void ofxMackerel::setTimeout(const int seconds) {
    timeout = seconds;
}

void ofxMackerel::setInterval(const int seconds) {
    interval = seconds;
}

string ofxMackerel::loadId() {
    // mac
    string path = (string)getenv("HOME") + "/Library/mackerel-agent/id";

    if (ofFile::doesFileExist(path, false)) {
        return ofBufferFromFile(path).getText();
    }
    ofLogError("No id file");
    return "";
}

string ofxMackerel::postRequest(string url, string body) {
    // https://gist.github.com/jefftimesten/247717dcca0669090cfa
    using Poco::Net::HTTPSClientSession;
    using Poco::Net::HTTPRequest;
    using Poco::Net::HTTPResponse;
    using Poco::Net::HTTPMessage;
    using Poco::URI;
    using Poco::StreamCopier;
    using Poco::Exception;

    try {
        // prepare session
        URI uri(url);
        HTTPSClientSession session(uri.getHost(), uri.getPort());
        session.setTimeout(Poco::Timespan(timeout, 0));

        // prepare path
        string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";

        // send request
        HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);
        req.setContentType("application/json");
        req.set("X-Api-Key", apiKey);
        req.setContentLength(body.length());

        // sends request, returns open stream
        std::ostream& os = session.sendRequest(req);
        os << body;  // sends the body
        // req.write(std::cout);  // print out request
        // cout << body << endl;

        // get response
        HTTPResponse res;

        istream &is = session.receiveResponse(res);
        stringstream ss;
        StreamCopier::copyStream(is, ss);

        ofLogVerbose() << ofToString(res.getStatus())
        << ":" << ss.str() << endl;

        return ss.str();
    }
    catch (Exception &ex) {
        cerr << ex.displayText() << endl;
        return "";
    }
}

void ofxMackerel::sendAllMetrics() {
    using Poco::JSON::Object;
    using Poco::JSON::Array;

    int epoch = static_cast<int>(ofGetUnixTime());
    Array params;
    for (auto& metric : metricsQueue) {
        Object m;
        m.set("hostId", id);
        m.set("name", metric.first);
        m.set("time", epoch);
        m.set("value", metric.second);
        params.add(m);
    }
    metricsQueue.clear();

    std::ostringstream os;
    params.stringify(os);
    string body = os.str();
    ofLogVerbose() << "ofxMackerel post "<< body << endl;

    auto th = std::thread([=]{
        postRequest(mackerel_origin+"/api/v0/tsdb", body);
    });
    th.detach();

    lastPostTime = epoch;
}
