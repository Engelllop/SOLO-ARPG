#pragma once

#include "CoreMinimal.h"

/**
 * Credenciales de operador local (herramienta / QA). La contraseÃ±a vive solo en el .cpp.
 * No sustituye seguridad en red; cualquier binario puede inspeccionar el cliente.
 */
namespace SOLOAdminAuth
{
	/** Solo uso interno del mÃ³dulo SOLOCore (no exportar como API del mÃ³dulo). */
	bool IsAdminUsername(const FString& Username, FString& OutCanonicalName);

	bool VerifyAdminPassword(const FString& Password);
}
