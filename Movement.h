class Movement {
public:
    Movement(float slowrate = 3.f, float fastrate = 20.f) : slowRate(slowrate), fastRate(fastrate) { };

    bool hasMovement() const noexcept { 
        return isActive && accumTime >= 1 / getRate(); 
    }

    Movement& operator--(int) {
        accumTime -= 1 / getRate();
        return *this;
    }
    Movement& operator--() {
        accumTime -= 1 / getRate();
        return *this;
    }

    void setRate(float rate) noexcept { slowRate = rate; }
    void setFastRate(float rate) noexcept { fastRate = rate; }
    void setActive(bool isactive = true) noexcept { isActive = isactive; };
    void setFast(bool isfast = true) noexcept {
        if (isfast != isFast) {
            resetTime();
        }
        isFast = isfast; 
    };

    void addTime(float t) noexcept { accumTime += t; };
    void resetTime() { accumTime = 0.f; }

private:
    float accumTime{ 0.f };
    float slowRate{ 0.f };
    float fastRate{ 0.f };

    bool isFast{ false };
    bool isActive{ false };
    float getRate() const noexcept { return isFast ? fastRate : slowRate; };

};