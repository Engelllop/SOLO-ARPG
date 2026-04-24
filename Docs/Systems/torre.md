# Torre (piso / progresión)

## Propósito

Centralizar el **estado global de la torre** (piso actual, notificación de cambios) y enlazarlo con el **progreso del jugador** en `FPlayerSaveData::TowerFloorReached` del `GameInstance`. La generación de salas, enemigos por piso y reglas de ascenso se irán colgando de este subsistema o de Actors del mapa de torre.

## Ubicación en código

| Pieza | Ruta |
|--------|------|
| Tipos compartidos | `Source/EMBERVEILCore/Public/Tower/EMBERVEILTowerTypes.h` |
| Subsistema | `Source/EMBERVEILCore/Public/Tower/EMBERVEILTowerSubsystem.h` |

## Datos y config

- `UEMBERVEILTowerSubsystem::CurrentFloorIndex` — piso lógico actual (≥ 0).
- Al subir de piso, `SetCurrentFloorIndex` actualiza `ActivePlayerData.TowerFloorReached` al máximo alcanzado.
- Delegado Blueprint `OnFloorChanged` para UI o lógica de mapa.

## Dependencias

- `UEMBERVEILGameInstance` para persistencia de sesión del personaje activo.
- Futuro: Data Assets de plantas, tablas de encuentros, integración con loot o quests.

## Extensiones

1. Definir **Data Assets** por planta (`FloorTag`, bioma, tablas de spawn).
2. Conectar **transición de nivel** (subnivel streaming o `OpenLevel`) cuando cambie el piso.
3. Guardar **run** o **seed** en SaveGame si el diseño lo requiere (roguelike).
