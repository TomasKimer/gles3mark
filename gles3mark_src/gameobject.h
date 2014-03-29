

#pragma once

#include <string>
#include <list>

class Color {

};

class Time {
public:
    static float deltaTime() { return 0; }
    
};

class Object {
    std::string name;

};

class Component : public Object {
public:    
    Component(GameObject *owner) : gameObject(owner) {}
    
    bool enabled;
    GameObject *gameObject;
};

class Behaviour : public Component {
    virtual void OnUpdate() {}
    virtual void OnCreate() {}
    virtual void OnDestroy() {}
};

class Material : public Object {
    
};

class Transform : public Component {
    
};

class Camera : public Component {
    
};

class Renderer : public Component {  // mesh
    
};

class GameObject : public Object {  // instanceof - rtti
    Transform transform;
    Renderer renderer;
    // collider
    std::list<Component*> attachedComponents;

public:
    void AddComponent(Component& component) {
        component.gameObject = this;
        attachedComponents.push_back(&component);
    }
};