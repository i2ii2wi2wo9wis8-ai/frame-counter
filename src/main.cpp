#include <Geode/Geode.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <Geode/modify/CCScene.hpp>
#include <deque>
#include <numeric>
#include <algorithm>

using namespace geode::prelude;

// ---------- Frame Counter Layer ----------
class FrameCounterLayer : public cocos2d::CCLayer {
public:
    static FrameCounterLayer* create() {
        auto ret = new FrameCounterLayer();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool init() override {
        if (!CCLayer::init()) return false;

        auto mod = Mod::get();
        m_updateInterval = mod->getSettingValue<float>("update_interval");
        m_windowSize = static_cast<size_t>(mod->getSettingValue<int64_t>("window_size"));
        if (m_windowSize < 10) m_windowSize = 10;

        m_label = CCLabelBMFont::create("FPS: --", "bigFont.fnt");
        m_label->setAnchorPoint({0.f, 1.f});
        m_label->setScale(0.45f);
        auto win = CCDirector::sharedDirector()->getWinSize();
        m_label->setPosition({5.f, win.height - 5.f});
        m_label->setZOrder(999);
        this->addChild(m_label);

        this->scheduleUpdate();
        return true;
    }

    void pushFrameTime(float dt) {
        if (dt <= 0.f || dt > 1.f) return;
        m_frameTimes.push_back(dt);
        while (m_frameTimes.size() > m_windowSize)
            m_frameTimes.pop_front();
    }

    float computeWindowFPS() const {
        if (m_frameTimes.empty()) return 0.f;
        float sum = std::accumulate(m_frameTimes.begin(), m_frameTimes.end(), 0.f);
        if (sum <= 0.f) return 0.f;
        return static_cast<float>(m_frameTimes.size()) / sum;
    }

    void update(float dt) override {
        m_accum += dt;
        if (m_accum < m_updateInterval) return;
        m_accum = 0.f;

        float fps = computeWindowFPS();
        if (fps > 0.f) {
            m_maxFPS = std::max(m_maxFPS, fps);
            m_minFPS = std::min(m_minFPS, fps);
            m_avgFPS = (m_avgFPS == 0.f) ? fps : (m_avgFPS * 0.9f + fps * 0.1f);
        }
        refreshLabel();
    }

    void refreshLabel() {
        if (!m_label) return;
        auto mod = Mod::get();

        std::string txt = fmt::format("FPS: {:.1f}", m_avgFPS);
        if (mod->getSettingValue<bool>("show_max"))
            txt += fmt::format(" | Max: {:.1f}", m_maxFPS);
        if (mod->getSettingValue<bool>("show_min") && m_minFPS < 9000.f)
            txt += fmt::format(" | Min: {:.1f}", m_minFPS);

        m_label->setString(txt.c_str());
    }

private:
    cocos2d::CCLabelBMFont* m_label = nullptr;
    std::deque<float> m_frameTimes;
    size_t m_windowSize = 60;
    float m_accum = 0.f;
    float m_updateInterval = 0.25f;
    float m_maxFPS = 0.f;
    float m_minFPS = 9999.f;
    float m_avgFPS = 0.f;
};

static FrameCounterLayer* g_counter = nullptr;

// ---------- Hook: her frame'de gercek dt'yi yakala ----------
class $modify(CCDirector) {
    void drawScene() {
        auto now = std::chrono::steady_clock::now();
        static auto last = now;
        float dt = std::chrono::duration<float>(now - last).count();
        last = now;

        if (g_counter) g_counter->pushFrameTime(dt);
        CCDirector::drawScene();
    }
};

// ---------- Hook: her sahnede sayaci yerlestir ----------
class $modify(CCScene) {
    void onEnter() {
        CCScene::onEnter();
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;

        if (!g_counter) {
            g_counter = FrameCounterLayer::create();
        }
        if (g_counter && !g_counter->getParent()) {
            this->addChild(g_counter, 9999);
        }
    }
};
