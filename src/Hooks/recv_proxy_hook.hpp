#pragma once
#include "../SDK.hpp"

class recv_prop_hook
{
public:
	recv_prop_hook(sdk::RecvProp* prop, const sdk::RecvVarProxyFn proxy_fn) :
		m_property(prop),
		m_original_proxy_fn(prop->m_ProxyFn)
	{
		set_proxy_function(proxy_fn);
	}

	~recv_prop_hook()
	{
		m_property->m_ProxyFn = m_original_proxy_fn;
	}

	auto get_original_function() const -> sdk::RecvVarProxyFn
	{
		return m_original_proxy_fn;
	}

	auto set_proxy_function(const sdk::RecvVarProxyFn proxy_fn) const -> void
	{
		m_property->m_ProxyFn = proxy_fn;
	}

private:
	sdk::RecvProp* m_property;
	sdk::RecvVarProxyFn m_original_proxy_fn;
};
