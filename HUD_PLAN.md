# SOLO HUD â€” Plan de ImplementaciÃ³n V4 (Islas Flotantes)

## Estado Actual
El proyecto SOLO YA tiene todos los widgets C++ creados:
- âœ… WG_MainHUD â€” contenedor principal
- âœ… WG_VitalBars â€” HP/MP/Stamina interpolado
- âœ… WG_ExperienceBar â€” XP con nivel
- âœ… WG_Minimap â€” con dots de jugador/enemigos/quests/landmarks
- âœ… WG_Hotbar â€” slots con iconos, cantidades, cooldowns
- âœ… WG_PartyFrames â€” frames de grupo
- âœ… WG_BossHealthBar â€” barra de jefe
- âœ… WG_ComboCounter â€” contador de combo
- âœ… WG_AbilityBar â€” barra de habilidades
- âœ… WG_DamageNumber â€” nÃºmeros de daÃ±o flotantes
- âœ… WG_MainMenu â€” menÃº principal
- âœ… SOLOHUDBase â€” HUD base con pool de damage numbers
- âœ… SOLOHUDOverlayWidget â€” overlay con bindings al GAS

## Tu DiseÃ±o V4 (Islas Flotantes) vs Widgets Existentes

| Elemento | Tu diseÃ±o | Widget existente | QuÃ© falta |
|----------|-----------|-----------------|-----------|
| Avatar + HP/MP/Stamina | Sup izq, inline con icono circular, porcentajes | WG_VitalBars (barras separadas con etiquetas) | Re-estilizar a sketch, juntar en un solo bloque |
| XP Bar | Sup centro, barra delgada con Lv.12, texto "2840/6800xp" | WG_ExperienceBar (barra + texto) | OK, solo ajustar posiciÃ³n |
| Minimapa circular | Sup der, redondo con dots | WG_Minimap (no sabemos si circular o cuadrado) | Verificar si ya es circular, sino agregar opciÃ³n |
| Quest Log | Der, hover expandible "La CÃ¡mara de Cenizas" | NO existe | **NUEVO** |
| Battle Log | Inf izq, hover expandible con colores por tipo | NO existe | **NUEVO** |
| Hotbar | Inf der, Q/W/E/R + 1/2 con cooldowns | WG_Hotbar (slots con keybind + cooldown) | OK, ajustar estilo sketch |
| Character Silhouette | Centro, SVG de "Aelindra" con capa | NO existe | **NUEVO** o decorativo |
| Status Effects | Mini iconos junto al avatar | NO existe separado | **NUEVO** o integrar en VitalBars |
| Sketchy style | Bordes dobles, filtro SVG sketchy, colores tenues | Estilo UE5 default | **NUEVO** â€” tema visual |

## Widgets que FALTA Crear (C++)

### 1. WG_QuestLogWidget
- Lista de misiones activas (principal + secundarias)
- Hover expandible con objetivos (â—‹) y completados (âœ“)
- BotÃ³n "[M] ver todas las misiones"

### 2. WG_BattleLogWidget
- Array de strings con tipo (dmg/heal/buff/sys)
- Ãšltimos 2 visibles por defecto
- Hover para expandir log completo
- Colores por tipo

### 3. WG_StatusEffectsWidget
- Lista de buffs/debuffs activos con icono + duraciÃ³n

### 4. HUD V4 Compositor (WBP_HUD_V4)
- Widget de alto nivel que combina todos los elementos en layout "islas flotantes"
- Posicionamiento absoluto

## Widgets que ya existen y solo necesitan ajuste estÃ©tico

### WG_VitalBars
- Agregar: avatar circular a la izquierda, nombre "Aelindra"
- Cambiar etiquetas a formato porcentaje
- Usar colores: HP=#6aa87c, MP=#6a7ce8, STA=#e8c46a

### WG_ExperienceBar
- Cambiar texto a "Lv.{level} â€” {current}/{required} xp"
- Mover a posiciÃ³n superior central

### WG_Minimap
- Si no es circular, agregar opciÃ³n circular
- Agregar dots de enemigos (rojos) con posiciones relativas

### WG_Hotbar
- Asegurar que los keybinds (Q/W/E/R/1/2) se muestren
- Cooldowns overlay visibles

## Archivos a crear

### Public/UI/Widgets/
- `WG_QuestLogWidget.h`
- `WG_BattleLogWidget.h`
- `WG_StatusEffectsWidget.h`

### Private/UI/Widgets/
- `WG_QuestLogWidget.cpp`
- `WG_BattleLogWidget.cpp`
- `WG_StatusEffectsWidget.cpp`

## Archivos a MODIFICAR

### WG_VitalBars.h/.cpp
- Agregar avatar icon + nombre
- Mostrar porcentaje numÃ©rico en barras

### WG_Minimap.h/.cpp
- OpciÃ³n de forma circular

### SOLOHUDOverlayWidget.h/.cpp
- Agregar binds para nuevos widgets
- Layout V4 (islas flotantes)
