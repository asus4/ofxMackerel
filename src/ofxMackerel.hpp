//
//  ofxMackerel.hpp
//  Wrapper for mackerel.io
//  https://mackerel.io/
//
//  Copyright 2015 asus4
//
//

#pragma once

#include <string>
#include <map>
#include "ofMain.h"

namespace ofx {
namespace mackerel {

class Mackerel {
 public:
    ~Mackerel() {}
    Mackerel();

    void setup(const std::string apiKey, const std::string id = "");
    void queueMetrics(const std::string metrics, const float value);

    // settings
    void setTimeout(const int seconds);
    void setInterval(const int seconds);

 protected:
    std::string apiKey;
    std::string id;
    std::map<std::string, float> metricsQueue;
    unsigned int lastPostTime;
    int timeout;
    int interval;

    // private
    std::string loadId();
    std::string postRequest(string url, string body);
    void sendAllMetrics();
};

}  // namespace mackerel
}  // namespace ofx

using ofxMackerel = ofx::mackerel::Mackerel;
