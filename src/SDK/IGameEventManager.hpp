#pragma once

namespace sdk
{
	class IGameEvent
	{
	public:
		virtual ~IGameEvent() {};
		virtual const char* GetName() const = 0;

		virtual bool IsReliable() const = 0;
		virtual bool IsLocal() const = 0;
		virtual bool IsEmpty(const char* keyname = NULL) = 0;

		virtual bool GetBool(const char* keyname = NULL, bool default_value = false) = 0;
		virtual int GetInt(const char* keyname = NULL, int default_value = 0) = 0;
		virtual uint64_t GetUint64(const char* keyname = NULL, uint64_t default_value = 0) = 0;
		virtual float GetFloat(const char* keyname = NULL, float default_value = 0.0f) = 0;
		virtual const char* GetString(const char* keyname = NULL, const char* default_value = "") = 0;
		virtual const wchar_t* GetWString(const char* keyname = NULL, const wchar_t* default_value = L"") = 0;
		virtual const void* GetPtr(const char* keyname = NULL, const void* default_values = NULL) = 0;

		virtual void SetBool(const char* keyname, bool value) = 0;
		virtual void SetInt(const char* keyname, int value) = 0;
		virtual void SetUint64(const char* keyname, uint64_t value) = 0;
		virtual void SetFloat(const char* keyname, float value) = 0;
		virtual void SetString(const char* keyname, const char* value) = 0;
		virtual void SetWString(const char* keyname, const wchar_t* value) = 0;
		virtual void SetPtr(const char* keyname, const void* value) = 0;
	};

	class IGameEventListener
	{
	public:
		virtual ~IGameEventListener(void) {};
		virtual void FireGameEvent(IGameEvent* event) = 0;
		virtual int GetEventDebugID() { return 42; }
	};

	class IGameEventManager
	{
	public:
		virtual ~IGameEventManager(void) {};
		virtual int LoadEventsFromFile(const char* filename) = 0;
		virtual void Reset() = 0;
		virtual bool AddListener(IGameEventListener* listener, const char* name, bool serverside) = 0;
		virtual bool FindListener(IGameEventListener* listener, const char* name) = 0;
		virtual void RemoveListener(IGameEventListener* listener) = 0;
		virtual void AddListenerGlobal(IGameEventListener* listener, bool serverside) = 0;
		virtual IGameEvent* CreateEvent(const char* name, bool force = false, int* cookie = nullptr) = 0;
		virtual bool FireEvent(IGameEvent* event, bool bDontBroadcast = false) = 0;
		virtual bool FireEventClientSide(IGameEvent* event) = 0;
		virtual IGameEvent* DuplicateEvent(IGameEvent* event) = 0;
		virtual void FreeEvent(IGameEvent* event) = 0;
		virtual bool SerializeEvent(IGameEvent* event, bf_write* buffer) = 0;
		virtual IGameEvent* UnserializeEvent(bf_read* buffer) = 0;
		virtual KeyValues* GetEventDataTypes(IGameEvent* event) = 0;
	};
}