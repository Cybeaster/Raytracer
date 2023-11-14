#pragma once
#include <memory>
#include <vector>

#include "Hittable.h"


class OHittableList : public IHittable {
public:
    OHittableList() = default;

    OHittableList(const std::shared_ptr<IHittable>&Object) {
        Objects.push_back(Object);
    }

    void Add(const std::shared_ptr<IHittable>&Object) {
        Objects.push_back(Object);
    }

    void Clear() {
        Objects.clear();
    }

    bool Hit(const ORay&Ray, float TMin, float TMax, SHitRecord&OutHitRecord) const override {
        bool HitAnything = false;
        auto ClosestSoFar = TMax;

        SHitRecord TempHitRecord;
        for (const auto&Object: Objects) {
            if (Object->Hit(Ray, TMin, ClosestSoFar, TempHitRecord)) {
                HitAnything = true;
                ClosestSoFar = TempHitRecord.T;
                OutHitRecord = TempHitRecord;
            }
        }

        return HitAnything;
    }

private:
    std::vector<std::shared_ptr<IHittable>> Objects;
};

