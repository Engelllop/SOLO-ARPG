# MenÃº principal y perfil local

## PropÃ³sito

Dar un **punto de entrada antes del gameplay** en single-player: captura de **nombre visible**, persistencia mÃ­nima en disco y transiciÃ³n al mapa de juego. **No sustituye** a Epic Online Services ni a cuentas online; es la capa de **identidad local** (fase 0).

## UbicaciÃ³n en cÃ³digo

| Pieza | Ruta |
|--------|------|
| Widget de menÃº (Slate mÃ­nimo) | `Source/SOLOCore/Public/UI/Widgets/WG_MainMenu.h` |
| GameMode de menÃº | `Source/SOLOCore/Public/GameFramework/SOLOMainMenuGameMode.h` |
| PlayerController de menÃº (cursor + input UI) | `Source/SOLOCore/Public/Player/SOLOMenuPlayerController.h` |
| Subsistema de perfil | `Source/SOLOCore/Public/Profile/SOLOProfileSubsystem.h` |
| SaveGame de perfil | `Source/SOLOCore/Public/Profile/SOLOLocalProfileSave.h` |
| Viaje al gameplay | `USOLOGameInstance::TravelToGameplayMap()` |

## Datos y config

- **Slot de guardado:** `SOLO_LocalProfile` (Ã­ndice de usuario `0`), vÃ­a `UGameplayStatics::SaveGameToSlot`.
- **Nombre en sesiÃ³n:** al confirmar en el menÃº se escribe en `USOLOProfileSubsystem` y se copia a `USOLOGameInstance::ActivePlayerData.CharacterName`.
- **Mapa de gameplay por defecto:** propiedad `GameplayMapPackage` en `USOLOGameInstance` (`/Game/Maps/L_Gameplay` hasta que el mapa exista en `Content/`).
- **Arranque del editor / paquete:** `Config/DefaultEngine.ini` â†’ `GameDefaultMap=/Engine/Maps/Entry`, `GlobalDefaultGameMode=ASOLOMainMenuGameMode`.

## Dependencias

- `UMG`, `Slate` (widget de menÃº construido en cÃ³digo).
- Sin EOS ni `OnlineSubsystem` para este flujo.

## Cuentas admin locales (QA / operador)

- Usuarios vÃ¡lidos: **`Admin0` â€¦ `Admin10`** (prefijo `Admin` sin distinguir mayÃºsculas; nÃºmero en base decimal sin ceros a la izquierda).
- ContraseÃ±a: **`89722729`** (solo en cliente; definida en `SOLOAdminAuth.cpp`).
- En el menÃº, el campo de **contraseÃ±a es opcional**. Si se rellena, debe coincidir usuario admin + contraseÃ±a o se rechaza el inicio.
- La sesiÃ³n admin activa `USOLOGameInstance::bAdminUnrestrictedAccess` y `AdminSessionAccountName`. En **gameplay**, comprobar `HasAdminUnrestrictedAccess()` antes de aplicar bloqueos (inventario, torre, progresiÃ³n, etc.). No se marca admin en el SaveGame del perfil: al relanzar el juego hay que volver a autenticar si se usÃ³ contraseÃ±a.

## Extensiones

1. Sustituir `UWG_MainMenu` por un **Widget Blueprint** hijo con arte (mantener la clase C++ como base o migrar lÃ³gica a Blueprint).
2. AÃ±adir **ajustes de audio / grÃ¡ficos** en el mismo mapa de menÃº o en pestaÃ±as del widget.
3. Cuando exista **auth online**, leer el nombre de cuenta y fusionarlo con el perfil local (no borrar el slot local sin decisiÃ³n de diseÃ±o).
