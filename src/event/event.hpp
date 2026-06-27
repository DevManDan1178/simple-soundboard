#pragma once


enum class EventType {
    None,
    ToggleWheel, 
    WheelScroll,
    WheelSelected, 
    ToggleUI, 
    ConfigChange,
    StopAllSounds,
    ConfigSaved
};

struct Event {
    virtual EventType getType() const = 0;
    virtual ~Event() = default;
};

struct StopAllSoundsEvent : Event {
    EventType getType() const override {
        return EventType::StopAllSounds;
    }
    StopAllSoundsEvent() = default;
};

struct ConfigChangeEvent : Event {
    EventType getType() const override {
        return EventType::ConfigChange;
    }
    ConfigChangeEvent() = default;
};

struct ConfigSavedEvent : Event {
    EventType getType() const override {
        return EventType::ConfigSaved;
    }
    ConfigSavedEvent() = default;
};

struct WheelSelectedEvent : Event {
    EventType getType() const override {
        return EventType::WheelSelected;
    }

    int wheelIdx;
    WheelSelectedEvent(int idx): wheelIdx(idx) {};
};

struct ToggleWheelEvent : Event {
    EventType getType() const override {
        return EventType::ToggleWheel;
    }
    bool visible;
    ToggleWheelEvent(bool visible): visible(visible) {};
};

struct WheelScrollEvent : Event {
    EventType getType() const override {
        return EventType::WheelScroll;
    }
    bool scrollDirection; // (True, False) => (Up, Down)
    WheelScrollEvent(bool scrollDirection) : scrollDirection(scrollDirection) {};
};

struct ToggleUIEvent : Event {
    EventType getType() const override {
        return EventType::ToggleUI;
    }
    ToggleUIEvent() = default;
};