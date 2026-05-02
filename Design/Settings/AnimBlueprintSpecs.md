// AnimBlueprint Specifications — Jefes N3-N10
// Para crear en UE5: Content Browser → AnimBlueprint → hereda de clase correspondiente

// ============================
// N3 — LeviathanBoss
// ============================
// BP: ABP_N3_Leviathan
// Target: ASOLON3_LeviathanBoss
// Skeleton: (importar malla propia — criatura acuática con espejos)
// States:
//   Idle — flotando, pulsación lenta
//   Phase1_SpawnReflejo — animación de abrir pecho, emite reflejo
//   Phase2_Broken — espejos rotos visibles, movimientos erráticos
//   Phase3_Exposed — vulnerable, ataque desesperado
// Slots:
//   Slot_Attack_ResonanceWave
//   Slot_Attack_MirrorBeam
//   Slot_Attack_WaterSurge

// ============================
// N3 — LeviathanReflejo
// ============================
// BP: ABP_N3_Reflejo
// Target: ASOLON3_LeviathanReflejo
// Skeleton: same as Leviathan (escala 0.8x, semitransparente)
// States:
//   Chase — persigue jugador
//   Attack — mismo ataque que Leviathan (débil)
//   Death — se desvanece en partículas

// ============================
// N4 — ColosoHambre
// ============================
// BP: ABP_N4_Coloso
// Target: ASOLON4_ColosoHambre
// Skeleton: humanoide enorme (escala 5x), huesudo, boca abierta
// States:
//   IdleWander — deambula lentamente, arrastra pies
//   Attack_Slow — levanta brazo, golpe lento (3s de windup)
//   Attack_GroundSmash — golpe al piso con ambas manos
//   Death — cae de rodillas, se desmorona
// Slots:
//   Slot_Attack_HungerWave
//   Slot_Attack_GroundSmash

// ============================
// N5 — Carnicero
// ============================
// BP: ABP_N5_Carnicero
// Target: ASOLON5_Carnicero
// Skeleton: humanoide (2.2x), overoles de carnicero, cuchillo
// States:
//   Idle — respiración pesada, limpia cuchillo
//   Phase1_Combat — pasos lentos, swings amplios
//   Phase2_Enrage — más rápido, ataques dobles
//   Death — se arrodilla, deja caer cuchillo
// Slots:
//   Slot_Attack_CleaverSwing
//   Slot_Attack_Grab
//   Slot_Attack_Enrage

// ============================
// N6 — CoroDeJade (Capataz)
// ============================
// BP: ABP_N6_Capataz
// Target: ASOLON6_CoroDeJade
// Skeleton: humanoide (1.8x), ropa industrial, látigo
// States:
//   Idle — postura autoritaria, mira reloj
//   Patrol — camina entre ChildSingers
//   Combat — látigo en mano, agresivo
//   Phase2_Desperate — máscara social rota, frenético
// Slots:
//   Slot_Attack_Lash
//   Slot_Attack_CrystalThrow

// ============================
// N7 — Costurera
// ============================
// BP: ABP_N7_Costurera
// Target: ASOLON7_Costurera
// Skeleton: humanoide anciana (1.5x), aguja gigante como bastón
// States:
//   Phase1_Idle — sentada cosiendo, no mira al jugador
//   Phase1_ThreadBreak — reacciona cuando rompen hilos (2 variantes)
//   Phase3_Combat — de pie, aguja en mano, rencorosa
//   Intangible — se vuelve etérea, semi-transparente
//   Death — cae, último hilo se rompe
// Slots:
//   Slot_Attack_Needle
//   Slot_Attack_Thread
//   Slot_Attack_DanceOfPatches
//   Slot_Attack_SummonMarionette

// ============================
// N8 — CriadaBronce
// ============================
// BP: ABP_N8_Criada
// Target: ASOLON8_CriadaBronce
// Skeleton: elfa delgada (2.0x), brazos largos, tijeras como manos
// States:
//   Idle — podando plantas imaginarias
//   Combat — tijeras abiertas, postura de jardinero
//   Phase2_Frenetic — más rápido, menos precisión
//   Death — cae, pide nombre, sonríe o no
// Slots:
//   Slot_Attack_Prune
//   Slot_Attack_Graft
//   Slot_Attack_Harvest

// ============================
// N9 — VerdugoPlata
// ============================
// BP: ABP_N9_Verdugo
// Target: ASOLON9_VerdugoPlata
// Skeleton: humanoide (2.0x), túnica negra
// States:
//   Idle — sentado, máscara en mano
//   Phase1_Masked — máscara puesta, movimientos mecánicos
//   Phase2_Exposed — máscara rota, cara de niño, movimientos erráticos
//   Death — cae lento, espíritu se separa
// Slots:
//   Slot_Attack_ConfessionBlow
//   Slot_Attack_TongueCut
//   Slot_Attack_TortureWheel
//   Slot_Phase2_AttackPainMemory
//   Slot_Phase2_AttackBrainNeedle

// ============================
// N10 — PlayerClone
// ============================
// BP: ABP_N10_Clone
// Target: ASOLON10_PlayerClone (o BP genérico)
// Skeleton: SAME as player character (copia exacta)
// States:
//   Mirror — copia animaciones del jugador con 0.5s delay
//   Idle — espera, postura idéntica al jugador
//   Surrender — baja el arma, se desvanece
// NOTA: Este AnimBP es especial. En vez de transitions normales,
//       usa un Tick event que lee la animación actual del jugador
//       y la reproduce con delay. Es un espejo animado.

// ============================
// N10 — Arquitecto (No AnimBP necesario)
// ============================
// Usa AnimationSequence simple: sentado escribiendo en teclado.
// No necesita state machine. Solo idle loop + reacciones faciales
// (opcional, se puede hacer con Morph Targets).
