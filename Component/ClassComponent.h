#pragma once
#include "Class/ClassBase.h"
#include "Component/Components.h"
#include <iostream>
#include <ostream>
#include <sstream>


namespace TTOT::Components {
class ClassComponent : public Component {
private:
  std::unique_ptr<TTOT::Class::ClassBase> classInfo = nullptr;

public:
  ClassComponent(std::unique_ptr<TTOT::Class::ClassBase> classInfo)
      : classInfo(std::move(classInfo)) {}
  uint32_t GetTypeID() const override {
    return GetComponentTypeID<ClassComponent>();
  }
  std::string ToString() const override {
    ostringstream oss;
    oss << "클래스: " << classInfo->GetName() << endl;
    oss << "특징: " << classInfo->GetDesc() << endl;
    return oss.str();
  }
  TTOT::Class::ClassBase *GetClassInfo() const { return classInfo.get(); }
};
} // namespace TTOT::Components