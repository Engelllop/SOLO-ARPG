# Changelog

Formato basado en [Keep a Changelog](https://keepachangelog.com/es-ES/1.0.0/).  
Las versiones siguen el criterio del equipo (semver de producto, milestone, o fecha).

## [Sin publicar]

### AÃ±adido
- MenÃº de arranque (`ASOLOMainMenuGameMode`, `UWG_MainMenu`, `ASOLOMenuPlayerController`) y perfil local (`USOLOProfileSubsystem` + `USOLOLocalProfileSave`).
- NÃºcleo de torre: `USOLOTowerSubsystem` y tipos en `SOLOTowerTypes.h`.
- DocumentaciÃ³n en `Docs/Systems/menu-y-perfil-local.md` y `Docs/Systems/torre.md`.
- `DefaultEngine.ini`: arranque en `/Engine/Maps/Entry` con GameMode de menÃº; `USOLOGameInstance::TravelToGameplayMap()` abre el mapa de gameplay con `ASOLOGameMode`.
- Cuentas admin locales `Admin0`â€“`Admin10` con contraseÃ±a en `SOLOAdminAuth`; sesiÃ³n sin restricciones vÃ­a `HasAdminUnrestrictedAccess()` y campo de contraseÃ±a en `UWG_MainMenu`.

### Cambiado
- *(Ejemplo: balance de daÃ±o base en `Config/Balance/Combat/`.)*

### Corregido
- *(Ejemplo: crash al abrir el mapa en multijugador.)*

### Obsoleto / eliminado
- *(Ejemplo: API antigua de loot; usar `SOLOLootComponent` v2.)*

---

## Plantilla de entrada (copiar debajo de `[Sin publicar]`)

```markdown
### AÃ±adido / Cambiado / Corregido
- DescripciÃ³n breve en una lÃ­nea; enlace a PR o commit si aplica.
```

**Buenas prÃ¡cticas**

- Una lÃ­nea por cambio notable; detalle tÃ©cnico en `Docs/Systems/` si hace falta.
- Mencionar rutas o sistemas solo cuando ayuden a localizar el cambio.
- Antes de release, renombrar `[Sin publicar]` a la versiÃ³n o fecha acordada y abrir un nuevo bloque `[Sin publicar]` arriba.
