# SOLO — Sunlight of the Last Online

**SOLO Online** is a dark fantasy ARPG where the world ended in sunlight.

Production-grade Action RPG built on **Unreal Engine 5.7** in C++. Features 84 gameplay systems spanning the full GAS (Gameplay Ability System) stack, AI behavior trees, real-time economy, guild mechanics, and a complete HUD framework.

![C++](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Unreal Engine 5](https://img.shields.io/badge/Unreal_Engine_5.7-0E1128?style=flat-square&logo=unrealengine&logoColor=white)
![GAS](https://img.shields.io/badge/Gameplay_Ability_System-blue?style=flat-square)
![Status](https://img.shields.io/badge/Status-In_Development-yellow?style=flat-square)

> **Note:** This repository contains C++ source code only. Binary assets (meshes, textures, audio) are excluded. Engine binaries and intermediate files are gitignored.

## Architecture

### Core Design Principle: PlayerState-Centric Systems

All persistent gameplay systems live in **ASOLOPlayerState** â€” not in the character â€” so they survive respawn and can be queried without a valid pawn:

```
ASOLOPlayerState
â”œâ”€â”€ UInventoryComponent
â”œâ”€â”€ UQuestComponent
â”œâ”€â”€ USkillMasteryComponent
â”œâ”€â”€ UCraftingComponent
â”œâ”€â”€ UAbilityBarComponent
â”œâ”€â”€ UGuildComponent
â””â”€â”€ UAbilitySystemComponent  â† GAS lives here
```

This means HUD widgets, other players, and server code can always safely access game state without checking if a character exists.

### Module Structure

| Module | Files | Description |
|--------|-------|-------------|
| `SOLO` | Runtime | Engine target, build rules |
| `SOLOCore` | 84 headers / 64 .cpp | All gameplay systems |

## Systems (84 total)

### Ability System (GAS)
| Class | Type | Description |
|-------|------|-------------|
| `GA_BasicAttack` | GameplayAbility | Standard melee/ranged attack |
| `GA_ChargedAttack` | GameplayAbility | Hold-to-charge heavy strike |
| `GA_Dodge` | GameplayAbility | Directional dodge with i-frames |
| `GA_Ultimate` | GameplayAbility | Class ultimate ability |
| `GA_Sprint` | GameplayAbility | Speed boost with stamina drain |
| `GA_UseItem` | GameplayAbility | Consume item from hotbar |
| `GA_Interact` | GameplayAbility | NPC/SavePoint/trigger interaction |
| `GA_Block` | GameplayAbility | Parry window with damage reduction |
| `GE_DamageBase` | GameplayEffect | Root damage effect |
| `GE_StaminaDrain` | GameplayEffect | Stamina cost application |
| `GE_HealOverTime` | GameplayEffect | Periodic healing |
| `DamageExecCalc` | ExecutionCalculation | Final damage formula (stat-aware) |

### Character Systems
- `ASOLOPlayerCharacter` â€” movement, input forwarding to GAS, camera
- `ASOLOEnemyCharacter` â€” health, hit reactions, boss phase detection, `ULootComponent`

### AI
- Behavior Trees with custom Tasks, Decorators, Services
- Patrol â†’ Alert â†’ Combat state machine
- `EQS` environment queries for cover and combat positioning
- Target selection and threat management

### Inventory, Loot & Economy
- `UInventoryComponent` â€” item stacks, equipment slots, persistent save
- `ULootComponent` on enemy â€” table-driven drops with rarity weights
- `UMarketplaceComponent` + `UShopNPCComponent` â€” gold-based buy/sell
- 9 items defined (Weapons, Armor, Consumables, Materials)
- 3 LootTables (CommonEnemy, EliteEnemy, Boss)

### Quest & Crafting
- `UQuestComponent` â€” objectives, branching, completion callbacks via GameplayTags
- `UCraftingComponent` â€” recipe DataAssets, material validation, output dispatch

### Skill Mastery & Guild
- `USkillMasteryComponent` â€” XP per damage type, tier unlocks
- `UGuildComponent` â€” membership, rank, shared buff tags

### Tower Defense
- `ATowerBase` â€” modular tower with range, targeting, upgrade states

### World Systems
- `ASOLOSavePoint` â€” checkpoint with respawn logic
- `AEnemySpawner` â€” wave-based, supports density and cooldown config
- World Partition enabled on `L_OpenWorld`

### HUD & UI (11 widgets)
`WBP_VitalBars`, `WBP_BossHealthBar`, `WBP_ComboCounter`, `WBP_DamageNumbers`, `WBP_AbilityBar`, `WBP_Hotbar`, `WBP_Minimap`, `WBP_PartyFrames`, `WBP_ExperienceBar`, `WBP_MainHUD`, `WBP_MainMenu`

## Gameplay Tags

100+ tags covering:
- `State.*` â€” Attacking, Dodging, Dead, Staggered, Sprinting
- `Ability.*` â€” per-ability identification
- `DamageType.*` â€” Physical, Fire, Ice, Lightning, Poison
- `Race.*` / `Class.*` â€” character archetype tags
- `Event.*` â€” GAS event channels

## Plugins Used

| Plugin | Purpose |
|--------|---------|
| `GameplayAbilities` | GAS runtime |
| `EnhancedInput` | Input action system |
| `CommonUI` | HUD layer management |
| `OnlineSubsystem` | Multiplayer foundation |
| `Niagara` | VFX |
| `AIModule` | Behavior Trees, EQS |

## Getting Started

See [Docs/ONBOARDING.md](Docs/ONBOARDING.md) for full setup instructions.

**Requirements:**
- Unreal Engine 5.7 (install from Epic Games Launcher)
- Visual Studio 2022 with C++ workload, or Rider

```bash
git clone https://github.com/Engelllop/SOLO-arpg
# Right-click SOLO.uproject â†’ Generate Visual Studio project files
# Open SOLO.sln â†’ Build â†’ Run
```

## Documentation

| File | Description |
|------|-------------|
| [CONTRIBUTING.md](CONTRIBUTING.md) | Git workflow, PR rules, coding standards |
| [AGENTS.md](AGENTS.md) | AI agent context and system map |
| [Docs/ONBOARDING.md](Docs/ONBOARDING.md) | First-time setup and navigation guide |
| [Docs/ROADMAP_FASES.md](Docs/ROADMAP_FASES.md) | Phase roadmap |
| [Design/GDD/](Design/GDD/) | Game Design Document |

## License

See [LICENSE](LICENSE).
