#include "event/event_dispatcher.hpp"
#include <iostream>

std::unordered_map<int, EventListener> listeners;

int EventDispatcher::Subscribe(const EventListener& listener) {
    int listenerId = listeners.size();
    listeners[listenerId] = listener;
    return listenerId;
}

void EventDispatcher::Unsubscribe(int connectionId) {
    listeners.erase(connectionId);
}

void EventDispatcher::Emit(const Event& event) {
    for (const auto& [listenerId, listener] : listeners) {
        listener(event);
    }
}