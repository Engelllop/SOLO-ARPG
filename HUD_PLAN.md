# EMBERVEIL HUD — Plan de Implementación V4 (Islas Flotantes)

## Estado Actual
El proyecto EMBERVEIL YA tiene todos los widgets C++ creados:
- ✅ WG_MainHUD — contenedor principal
- ✅ WG_VitalBars — HP/MP/Stamina interpolado
- ✅ WG_ExperienceBar — XP con nivel
- ✅ WG_Minimap — con dots de jugador/enemigos/quests/landmarks
- ✅ WG_Hotbar — slots con iconos, cantidades, cooldowns
- ✅ WG_PartyFrames — frames de grupo
- ✅ WG_BossHealthBar — barra de jefe
- ✅ WG_ComboCounter — contador de combo
- ✅ WG_AbilityBar — barra de habilidades
- ✅ WG_DamageNumber — números de daño flotantes
- ✅ WG_MainMenu — menú principal
- ✅ EMBERVEILHUDBase — HUD base con pool de damage numbers
- ✅ EMBERVEILHUDOverlayWidget — overlay con bindings al GAS

## Tu Diseño V4 (Islas Flotantes) vs Widgets Existentes

| Elemento | Tu diseño | Widget existente | Qué falta |
|----------|-----------|-----------------|-----------|
| Avatar + HP/MP/Stamina | Sup izq, inline con icono circular, porcentajes | WG_VitalBars (barras separadas con etiquetas) | Re-estilizar a sketch, juntar en un solo bloque |
| XP Bar | Sup centro, barra delgada con Lv.12, texto "2840/6800xp" | WG_ExperienceBar (barra + texto) | OK, solo ajustar posición |
| Minimapa circular | Sup der, redondo con dots | WG_Minimap (no sabemos si circular o cuadrado) | Verificar si ya es circular, sino agregar opción |
| Quest Log | Der, hover expandible "La Cámara de Cenizas" | NO existe | **NUEVO** |
| Battle Log | Inf izq, hover expandible con colores por tipo | NO existe | **NUEVO** |
| Hotbar | Inf der, Q/W/E/R + 1/2 con cooldowns | WG_Hotbar (slots con keybind + cooldown) | OK, ajustar estilo sketch |
| Character Silhouette | Centro, SVG de "Aelindra" con capa | NO existe | **NUEVO** o decorativo |
| Status Effects | Mini iconos junto al avatar | NO existe separado | **NUEVO** o integrar en VitalBars |
| Sketchy style | Bordes dobles, filtro SVG sketchy, colores tenues | Estilo UE5 default | **NUEVO** — tema visual |

## Widgets que FALTA Crear (C++)

### 1. WG_QuestLogWidget
- Lista de misiones activas (principal + secundarias)
- Hover expandible con objetivos (○) y completados (✓)
- Botón "[M] ver todas las misiones"

### 2. WG_BattleLogWidget
- Array de strings con tipo (dmg/heal/buff/sys)
- Últimos 2 visibles por defecto
- Hover para expandir log completo
- Colores por tipo

### 3. WG_StatusEffectsWidget
- Lista de buffs/debuffs activos con icono + duración

### 4. HUD V4 Compositor (WBP_HUD_V4)
- Widget de alto nivel que combina todos los elementos en layout "islas flotantes"
- Posicionamiento absoluto

## Widgets que ya existen y solo necesitan ajuste estético

### WG_VitalBars
- Agregar: avatar circular a la izquierda, nombre "Aelindra"
- Cambiar etiquetas a formato porcentaje
- Usar colores: HP=#6aa87c, MP=#6a7ce8, STA=#e8c46a

### WG_ExperienceBar
- Cambiar texto a "Lv.{level} — {current}/{required} xp"
- Mover a posición superior central

### WG_Minimap
- Si no es circular, agregar opción circular
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
- Mostrar porcentaje numérico en barras

### WG_Minimap.h/.cpp
- Opción de forma circular

### EMBERVEILHUDOverlayWidget.h/.cpp
- Agregar binds para nuevos widgets
- Layout V4 (islas flotantes)
