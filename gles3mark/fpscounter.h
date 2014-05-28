/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include <ostream>
#include <iomanip>

class FPSCounter {
public:
    FPSCounter();

    void Update(float deltaTime);

    float Current()     const { return current;     }
    float JustUpdated() const { return justUpdated; }

    friend std::ostream& operator << (std::ostream& o, const FPSCounter& v) {
        o << std::setw(7) << v.Current();  // std::fixed << std::setprecision(4)
        return o;
    }

private:
    float current;
    float totalElapsedTime;
    unsigned int frameCount;
    bool justUpdated;
};