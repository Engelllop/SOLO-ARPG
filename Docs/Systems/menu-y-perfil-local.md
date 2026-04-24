# Menú principal y perfil local

## Propósito

Dar un **punto de entrada antes del gameplay** en single-player: captura de **nombre visible**, persistencia mínima en disco y transición al mapa de juego. **No sustituye** a Epic Online Services ni a cuentas online; es la capa de **identidad local** (fase 0).

## Ubicación en código

| Pieza | Ruta |
|--------|------|
| Widget de menú (Slate mínimo) | `Source/EMBERVEILCore/Public/UI/Widgets/WG_MainMenu.h` |
| GameMode de menú | `Source/EMBERVEILCore/Public/GameFramework/EMBERVEILMainMenuGameMode.h` |
| PlayerController de menú (cursor + input UI) | `Source/EMBERVEILCore/Public/Player/EMBERVEILMenuPlayerController.h` |
| Subsistema de perfil | `Source/EMBERVEILCore/Public/Profile/EMBERVEILProfileSubsystem.h` |
| SaveGame de perfil | `Source/EMBERVEILCore/Public/Profile/EMBERVEILLocalProfileSave.h` |
| Viaje al gameplay | `UEMBERVEILGameInstance::TravelToGameplayMap()` |

## Datos y config

- **Slot de guardado:** `EMBERVEIL_LocalProfile` (índice de usuario `0`), vía `UGameplayStatics::SaveGameToSlot`.
- **Nombre en sesión:** al confirmar en el menú se escribe en `UEMBERVEILProfileSubsystem` y se copia a `UEMBERVEILGameInstance::ActivePlayerData.CharacterName`.
- **Mapa de gameplay por defecto:** propiedad `GameplayMapPackage` en `UEMBERVEILGameInstance` (`/Game/Maps/L_Gameplay` hasta que el mapa exista en `Content/`).
- **Arranque del editor / paquete:** `Config/DefaultEngine.ini` → `GameDefaultMap=/Engine/Maps/Entry`, `GlobalDefaultGameMode=AEMBERVEILMainMenuGameMode`.

## Dependencias

- `UMG`, `Slate` (widget de menú construido en código).
- Sin EOS ni `OnlineSubsystem` para este flujo.

## Cuentas admin locales (QA / operador)

- Usuarios válidos: **`Admin0` … `Admin10`** (prefijo `Admin` sin distinguir mayúsculas; número en base decimal sin ceros a la izquierda).
- Contraseña: **`89722729`** (solo en cliente; definida en `EMBERVEILAdminAuth.cpp`).
- En el menú, el campo de **contraseña es opcional**. Si se rellena, debe coincidir usuario admin + contraseña o se rechaza el inicio.
- La sesión admin activa `UEMBERVEILGameInstance::bAdminUnrestrictedAccess` y `AdminSessionAccountName`. En **gameplay**, comprobar `HasAdminUnrestrictedAccess()` antes de aplicar bloqueos (inventario, torre, progresión, etc.). No se marca admin en el SaveGame del perfil: al relanzar el juego hay que volver a autenticar si se usó contraseña.

## Extensiones

1. Sustituir `UWG_MainMenu` por un **Widget Blueprint** hijo con arte (mantener la clase C++ como base o migrar lógica a Blueprint).
2. Añadir **ajustes de audio / gráficos** en el mismo mapa de menú o en pestañas del widget.
3. Cuando exista **auth online**, leer el nombre de cuenta y fusionarlo con el perfil local (no borrar el slot local sin decisión de diseño).
