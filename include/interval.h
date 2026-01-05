#pragma once

class interval {
  public:
    float min, max;
    static const interval empty, universe;
    interval() : min(-infinity), max(infinity) {}
    interval(float min, float max) : min(min), max(max) {}
    
    float size() const {
        return max - min;
    }

    bool contains(float x) const {
        return min <= x && x <= max;
    }

    bool surrounds(float x) const {
        return min < x && x < max;
    }
};

const interval interval::empty =    interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
