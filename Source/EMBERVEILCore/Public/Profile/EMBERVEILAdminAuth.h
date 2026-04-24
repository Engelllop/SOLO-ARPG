#pragma once

#include "CoreMinimal.h"

/**
 * Credenciales de operador local (herramienta / QA). La contraseña vive solo en el .cpp.
 * No sustituye seguridad en red; cualquier binario puede inspeccionar el cliente.
 */
namespace EMBERVEILAdminAuth
{
	/** Solo uso interno del módulo EMBERVEILCore (no exportar como API del módulo). */
	bool IsAdminUsername(const FString& Username, FString& OutCanonicalName);

	bool VerifyAdminPassword(const FString& Password);
}
