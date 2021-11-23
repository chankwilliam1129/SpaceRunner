#pragma once
#include <list>
#include <string>
#include "../GameLib/GamePad.h"
#include "../GameLib/vector.h"
#include "../GameLib/quaternion.h"
#include "../GameLib/input.h"
#include "../GameLib/debug.h"
#include "../GameLib/singleton.h"

class Object;
class Component
{
public:
    Object* object = nullptr;
    //std::list<Object*> ChildList;

public:
    Component() {}
    virtual ~Component() {}

    virtual void Start() {}
    virtual void Update() {}
    virtual void TextureDraw() {}
    virtual void Draw() {}
    virtual void ImGuiUpdate() {};
};

class Object
{
public:
    bool isActive = true;
    std::list<Component*> ComponentList;
    std::string tag;
    std::string name;

    std::list<Object*> ChildList;
    Object* parent = nullptr;

public:
    Object()
    {
        ComponentList.clear();
        ChildList.clear();
    }
    ~Object()
    {
        isActive = false;
        for (auto com : ComponentList)
        {
            delete com;
        }
        ComponentList.clear();
        ChildList.clear();
    }

    void Update()
    {
        if (isActive)
        {
            for (auto com : ComponentList)
            {
                com->Update();
            }

            if (!ChildList.empty())
            {
                for (auto& it : ChildList)
                {
                    it->Update();
                }
            }
        }
    }

    void TextureDraw();
    void Draw();

    void ImGuiUpdate()
    {
#ifdef USE_IMGUI
        if (isActive)
        {
            if (ImGui::CollapsingHeader(name.c_str()))
            {
                for (auto& com : ComponentList)
                {
                    com->ImGuiUpdate();
                }
                ImGui::SetNextTreeNodeOpen(false, ImGuiSetCond_Once);

                if (!ChildList.empty())
                {
                    for (auto& it : ChildList)
                    {
                        it->ImGuiUpdate();
                    }
                }
            }
        }
#endif
    }

    //オブジェクトが持っているコンポーネントを取得
    template<class T>
    T* GetComponent()
    {
        for (auto com : ComponentList)
        {
            T* buff = dynamic_cast<T*>(com);
            if (buff != nullptr)
            {
                return buff;
            }
        }
        return nullptr;
    }

    //オブジェクトが持っているコンポーネントを追加
    template<class T>
    T* AddComponent()
    {
        T* buff = new T();
        buff->object = this;
        ComponentList.push_back(buff);
        buff->Start();
        return buff;
    }

    Object* AddChild(std::string name, Vector3 pos = Vector3(0, 0, 0), Vector3 rotation = Vector3(0, 0, 0), Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));
};
