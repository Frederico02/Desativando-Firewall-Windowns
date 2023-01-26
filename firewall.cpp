#include <iostream>
#include <Windows.h>
#include <comdef.h>
#include <Wbemidl.h>

int main()
{
    HRESULT hres;

    // Inicializa a COM
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        std::cout << "Falha ao inicializar a COM: " << hres << std::endl;
        return 1;
    }

    // Configura a segurança da COM
    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );
    if (FAILED(hres))
    {
        std::cout << "Falha ao configurar a segurança da COM: " << hres << std::endl;
        CoUninitialize();
        return 1;
    }

    // Cria um objeto locator
    IWbemLocator *locator = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID *)&locator
    );
    if (FAILED(hres))
    {
        std::cout << "Falha ao criar o objeto locator: " << hres << std::endl;
        CoUninitialize();
        return 1;
    }

    // Obtém a conexão com o namespace
    IWbemServices *services = NULL;
    hres = locator->ConnectServer(
        _bstr_t(L"ROOT\\SECURITYCENTER2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &services
    );
    if (FAILED(hres))
    {
        std::cout << "Falha ao obter a conexão com o namespace: " << hres << std::endl;
        locator->Release();
        CoUninitialize();
        return 1;
    }

    // Configura a segurança da conexão
    hres = CoSetProxyBlanket(
        services,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC
