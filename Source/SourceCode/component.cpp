#include "component.h"
#include "transform.h"
#include "mesh_render.h"

void Object::Draw()
{
    if (isActive)
    {
        for (auto com : ComponentList)
        {
            com->Draw();
        }
        if (!ChildList.empty())
        {
            for (auto& it : ChildList)
            {
                it->Draw();
            }
        }
    }
}

void Object::TextureDraw()
{
    if (isActive)
    {
        for (auto com : ComponentList)
        {
            
            com->TextureDraw();
            
        }
        if (!ChildList.empty())
        {
            for (auto& it : ChildList)
            {
                it->TextureDraw();
            }
        }
    }
}

Object* Object::AddChild(std::string name, Vector3 pos, Vector3 rotation, Vector3 scale)
{
	Object* obj = new Object;
	obj->name = name;
	obj->AddComponent<Transform>();
	obj->GetComponent<Transform>()->position = pos;
	obj->GetComponent<Transform>()->rotation = rotation;
	obj->GetComponent<Transform>()->scale = scale;
	obj->parent = this;
	ChildList.push_back(obj);
	return obj;
}