// Copyright(c) 2017 POLYGONTEK
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Precompiled.h"
#include "GoogleMobileAds/GoogleMobileAds.h"

class RewardBasedVideoAd {
public:
    void Request(const char *unitID, const char *testDevices);
    
    bool IsReady() const;
    
    void Present();
    
    void DidRewardUser(const char *rewardType, int rewardAmount);
    void DidReceiveAd();
    void DidOpen();
    void DidStartPlaying();
    void DidClose();
    void WillLeaveApplication();
    void DidFailToLoad(const char *errorDescription);
    
    static void RegisterLuaModule(LuaCpp::State *state, UIViewController<GADRewardBasedVideoAdDelegate> *viewController);
    
private:
    LuaCpp::Selector selector;
    UIViewController<GADRewardBasedVideoAdDelegate> *viewController;
};

extern RewardBasedVideoAd rewardBasedVideoAd;
