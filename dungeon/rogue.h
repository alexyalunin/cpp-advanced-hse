#pragma once

#include "dungeon.h"
#include <stdexcept>
#include <unordered_set>
#include <string>
#include <deque>

Room *FindFinalRoom(Room *starting_room) {
    std::unordered_set<Room *> rooms_searched;
    std::unordered_set<std::string> keys_received;
    std::unordered_set<Door *> locked_doors;
    std::deque<Room *> search;
    search.push_back(starting_room);
    while (!search.empty()) {
        if (!search.empty()) {
            Room *cur_room = search.front();
            search.pop_front();
            if (cur_room->IsFinal()) {
                return cur_room;
            }
            for (size_t i = 0; i < cur_room->NumKeys(); ++i) {
                if (!keys_received.contains(cur_room->GetKey(i))) {
                    keys_received.insert(cur_room->GetKey(i));
                }
            }
            for (size_t i = 0; i < cur_room->NumDoors(); ++i) {
                if (cur_room->GetDoor(i)->IsOpen()) {
                    if (!rooms_searched.contains(cur_room->GetDoor(i)->GoThrough())) {
                        search.push_back(cur_room->GetDoor(i)->GoThrough());
                        rooms_searched.insert(search.back());
                    }
                    continue;
                }
                for (auto &key : keys_received) {
                    if (cur_room->GetDoor(i)->TryOpen(key)) {
                        if (!rooms_searched.contains(cur_room->GetDoor(i)->GoThrough())) {
                            search.push_back(cur_room->GetDoor(i)->GoThrough());
                            rooms_searched.insert(search.back());
                        }
                        break;
                    }
                }
                if (!cur_room->GetDoor(i)->IsOpen()) {
                    locked_doors.insert(cur_room->GetDoor(i));
                }
            }
        } else {
            std::unordered_set<Door *> unlocked_doors;
            for (auto locked_door : locked_doors) {
                for (auto &key : keys_received) {
                    if (locked_door->TryOpen(key)) {
                        unlocked_doors.insert(locked_door);
                        if (!rooms_searched.contains(locked_door->GoThrough())) {
                            search.push_back(locked_door->GoThrough());
                            rooms_searched.insert(search.back());
                        }
                        break;
                    }
                }
            }
            for (auto unlocked_door : unlocked_doors) {
                locked_doors.erase(unlocked_door);
            }
            if (search.empty()) {
                break;
            }
        }
    }
    return nullptr;
}
