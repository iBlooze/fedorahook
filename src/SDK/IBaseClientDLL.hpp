#pragma once

namespace sdk
{
	class IBaseClientDLL
	{
	public:
		ClientClass* GetAllClasses()
		{
			return get_vfunc<ClientClass*(__thiscall *)(IBaseClientDLL*)>(this, 8)(this);
		}

		bool DispatchUserMessage(int msg_type, unsigned int arg1, unsigned int length, const void* data = nullptr)
		{
			using original_fn = bool(__thiscall*)(void*, int, unsigned int, unsigned int, const void*);
			return (*(original_fn**)this)[38](this, msg_type, arg1, length, data);
		}
	};

}