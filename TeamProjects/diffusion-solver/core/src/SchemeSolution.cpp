#include <limits>
#include "CoreUtils.hpp"
#include "SchemeSolution.hpp"

using namespace diffusioncore;

SchemeSolution::SchemeSolution() {
   mMinValue = std::numeric_limits<double>::signaling_NaN();
   mMaxValue = std::numeric_limits<double>::signaling_NaN();
}

SchemeSolution::SchemeSolution(const std::vector<SchemeLayer>& layers,
                               double minValue, double maxValue) {
   mMinValue = minValue;
   mMaxValue = maxValue;
   mLayers = layers;
}

SchemeSolution::SchemeSolution(const SchemeLayer& layer,
                               double minValue, double maxValue) {
   mMinValue = minValue;
   mMaxValue = maxValue;
   mLayers.push_back(layer);
}

SchemeSolution::~SchemeSolution() { }


SchemeLayer SchemeSolution::GetLayer(int index) const {
   return mLayers[index];
}

SchemeLayer SchemeSolution::GetFirstLayer() const {
   return mLayers.front();
}

SchemeLayer SchemeSolution::GetLastLayer() const {
   return mLayers.back();
}

SchemeLayer SchemeSolution::operator[] (int index) const {
   return GetLayer(index);
}

double SchemeSolution::GetMaximum() const {
   return mMaxValue;
}

double SchemeSolution::GetMinimum() const {
   return mMinValue;
}

int SchemeSolution::GetLayersCount() const {
   return (int)mLayers.size();
}
