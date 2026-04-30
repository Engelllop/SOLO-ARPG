# Torre (piso / progresiÃ³n)

## PropÃ³sito

Centralizar el **estado global de la torre** (piso actual, notificaciÃ³n de cambios) y enlazarlo con el **progreso del jugador** en `FPlayerSaveData::TowerFloorReached` del `GameInstance`. La generaciÃ³n de salas, enemigos por piso y reglas de ascenso se irÃ¡n colgando de este subsistema o de Actors del mapa de torre.

## UbicaciÃ³n en cÃ³digo

| Pieza | Ruta |
|--------|------|
| Tipos compartidos | `Source/SOLOCore/Public/Tower/SOLOTowerTypes.h` |
| Subsistema | `Source/SOLOCore/Public/Tower/SOLOTowerSubsystem.h` |

## Datos y config

- `USOLOTowerSubsystem::CurrentFloorIndex` â€” piso lÃ³gico actual (â‰¥ 0).
- Al subir de piso, `SetCurrentFloorIndex` actualiza `ActivePlayerData.TowerFloorReached` al mÃ¡ximo alcanzado.
- Delegado Blueprint `OnFloorChanged` para UI o lÃ³gica de mapa.

## Dependencias

- `USOLOGameInstance` para persistencia de sesiÃ³n del personaje activo.
- Futuro: Data Assets de plantas, tablas de encuentros, integraciÃ³n con loot o quests.

## Extensiones

1. Definir **Data Assets** por planta (`FloorTag`, bioma, tablas de spawn).
2. Conectar **transiciÃ³n de nivel** (subnivel streaming o `OpenLevel`) cuando cambie el piso.
3. Guardar **run** o **seed** en SaveGame si el diseÃ±o lo requiere (roguelike).
