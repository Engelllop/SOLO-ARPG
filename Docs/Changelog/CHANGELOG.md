# Changelog

Formato basado en [Keep a Changelog](https://keepachangelog.com/es-ES/1.0.0/).  
Las versiones siguen el criterio del equipo (semver de producto, milestone, o fecha).

## [Sin publicar]

### Añadido
- Menú de arranque (`AEMBERVEILMainMenuGameMode`, `UWG_MainMenu`, `AEMBERVEILMenuPlayerController`) y perfil local (`UEMBERVEILProfileSubsystem` + `UEMBERVEILLocalProfileSave`).
- Núcleo de torre: `UEMBERVEILTowerSubsystem` y tipos en `EMBERVEILTowerTypes.h`.
- Documentación en `Docs/Systems/menu-y-perfil-local.md` y `Docs/Systems/torre.md`.
- `DefaultEngine.ini`: arranque en `/Engine/Maps/Entry` con GameMode de menú; `UEMBERVEILGameInstance::TravelToGameplayMap()` abre el mapa de gameplay con `AEMBERVEILGameMode`.
- Cuentas admin locales `Admin0`–`Admin10` con contraseña en `EMBERVEILAdminAuth`; sesión sin restricciones vía `HasAdminUnrestrictedAccess()` y campo de contraseña en `UWG_MainMenu`.

### Cambiado
- *(Ejemplo: balance de daño base en `Config/Balance/Combat/`.)*

### Corregido
- *(Ejemplo: crash al abrir el mapa en multijugador.)*

### Obsoleto / eliminado
- *(Ejemplo: API antigua de loot; usar `EMBERVEILLootComponent` v2.)*

---

## Plantilla de entrada (copiar debajo de `[Sin publicar]`)

```markdown
### Añadido / Cambiado / Corregido
- Descripción breve en una línea; enlace a PR o commit si aplica.
```

**Buenas prácticas**

- Una línea por cambio notable; detalle técnico en `Docs/Systems/` si hace falta.
- Mencionar rutas o sistemas solo cuando ayuden a localizar el cambio.
- Antes de release, renombrar `[Sin publicar]` a la versión o fecha acordada y abrir un nuevo bloque `[Sin publicar]` arriba.
