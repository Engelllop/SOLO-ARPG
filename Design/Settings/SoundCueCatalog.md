// Sound Cues — organización por piso
// Todos los sonidos se importan a: /Game/Audio/
// Formato recomendado: WAV 44100Hz 16bit mono (diálogos) o stereo (ambiente)

// ============================
// N3 — Aelthar Music & SFX
// ============================
// Ambient: A3_Ambient_City.wav — arpa suave, pájaros, agua corriente
// Ambient: A3_Ambient_Basement.wav — goteo, silencio, respiración
// Ambient: A3_Ambient_Reveal.wav — distorsión lenta de arpa (transición)
// Boss: A3_Boss_Theme.wav — acuático, espejos resonando
// SFX: A3_Mirror_Break.wav — cristal rompiéndose
// SFX: A3_Mirror_Reflect.wav — sonido de rebote/redirección
// SFX: A3_Water_Surge.wav — oleada de agua
// SFX: A3_Child_Whisper.wav — susurro infantil (trigger en fuente)
// Voice: Garrik — líneas pregrabadas o texto-a-voz

// ============================
// N4 — Bosque Petrificado
// ============================
// NOTA: Sin música. Solo ambiente.
// Ambient: A4_Wind.wav — viento constante, bajo, sin melodía
// Ambient: A4_Trees_Creak.wav — madera/árboles crujiendo
// SFX: A4_Coloso_Breath.wav — respiración lenta y pesada
// SFX: A4_Heartbeat.wav — latido grave (CorazonHambre)
// SFX: A4_Eating.wav — sonidos de masticación (cabaña)
// SFX: A4_Hunger_Low.wav — estómago gruñendo (player)
// SFX: A4_Death_Rattle.wav — muerte por hambre

// ============================
// N5 — Thornwall
// ============================
// Ambient: A5_Street_Ambient.wav — murmullos, risas borrachas, música lejana
// Ambient: A5_Brothel_Inside.wav — música de burdel distorsionada
// Boss: A5_Carnicero_Theme.wav — ritmo de carnicería, cuchillos
// SFX: A5_Cleaver_Swing.wav — cuchillo cortando aire
// SFX: A5_Cage_Open.wav — jaula oxidada abriéndose
// SFX: A5_Girl_Cry.wav — llanto corto (no prolongado, no gratuito)
// SFX: A5_Drunk_Laugh.wav — risa de borracho

// ============================
// N6 — Cámaras de Jade
// ============================
// Ambient: A6_Children_Singing.wav — canto infantil en loop, capas múltiples
// NOTA: 3 variantes de canto (A, B, C) que se mezclan proceduralmente
// Ambient: A6_Factory_Hum.wav — maquinaria, eco de metal
// Ambient: A6_Quiet_Whimpering.wav — llanto suave (trigger cerca de ChildMiners)
// SFX: A6_Pick_Hit.wav — pico contra jade
// SFX: A6_Singer_Freed.wav — suspiro, canto se detiene
// SFX: A6_Capataz_Lash.wav — látigo
// SFX: A6_Jade_Crystalize.wav — jade formándose

// ============================
// N7 — Sahal
// ============================
// NOTA: Silencio es el sonido principal de Sahal.
// Ambient: A7_Silence.wav — silencio de habitación vacía con mínima resonancia
// Ambient: A7_Thread_Tension.wav — hilo tensándose (frecuente)
// SFX: A7_Needle_Pierce.wav — aguja penetrando carne (seco)
// SFX: A7_Thread_Snap.wav — hilo rompiéndose
// SFX: A7_Flesh_Tear.wav — carne desgarrándose (Vassen)
// SFX: A7_Knife_Slit.wav — cuchillo cortando garganta (Mira)
// SFX: A7_Marioneta_Step.wav — pasos de cuerpos arrastrados
// Boss: A7_Costurera_Theme.wav — agudo, violín distorsionado, incómodo
// Post-Boss: A7_Unravel.wav — todo descosiéndose (15s de duración)

// ============================
// N8 — Jardines Flotantes
// ============================
// Ambient: A8_Garden_Outside.wav — naturaleza, agua, viento suave (engañoso)
// Ambient: A8_Lab_Hum.wav — máquinas, líquidos, zumbido clínico
// Ambient: A8_Tank_Bubbles.wav — burbujeo de tanques
// SFX: A8_Prune_Shears.wav — tijeras de podar
// SFX: A8_Body_Fall.wav — cuerpo cayendo (tanques abiertos)
// SFX: A8_Sera_Rustle.wav — ramas moviéndose (Sera camina)
// Boss: A8_Criada_Theme.wav — ritmo de jardinería, metálico, frío

// ============================
// N9 — Argentia
// ============================
// Ambient: A9_Wind_Through_Cells.wav — viento en celdas vacías
// Ambient: A9_Drip.wav — goteo constante
// Ambient: A9_Anvil.wav — Oswin golpeando metal (lejano)
// SFX: A9_Guillotine_Drop.wav — cuchilla cayendo
// SFX: A9_Chain_Rattle.wav — cadenas arrastradas
// SFX: A9_Torture_Wheel.wav — rueda girando (Verdugo)
// SFX: A9_Scream_Archive.wav — grito de archivo (fase 2)
// Boss: A9_Verdugo_Theme.wav — marcha, tambor, grave

// ============================
// N10 — Final
// ============================
// Ambient: A10_MirrorHall.wav — eco infinito, distorsión
// SFX: A10_Footstep_Echo.wav — pasos que no coinciden con los tuyos
// Ambient: A10_Office_Hum.wav — ventilador, teclado, CPU (normalidad)
// SFX: A10_Keyboard_Typing.wav — tipeo (Arquitecto)
// SFX: A10_Clone_Match.wav — copy del player SFX con 0.5s delay
// Dialog: A10_Architect_VO — líneas del Arquitecto (si se graban)
// Endings:
//   A10_Ending_A_Explosion.wav — servidor explotando
//   A10_Ending_B_Door.wav — puerta abriéndose al exterior
//   A10_Ending_C_Chair.wav — silla girando, teclado

// ============================
// SFX Globales (Compartidos)
// ============================
// SFX: UI_Click.wav — clic de UI
// SFX: UI_Hover.wav — hover de botón
// SFX: UI_Quest_Update.wav — quest completada/actualizada
// SFX: UI_Item_Pickup.wav — item recogido
// SFX: UI_Level_Up.wav — subir de nivel
// SFX: Combat_Hit_Light.wav — golpe ligero
// SFX: Combat_Hit_Heavy.wav — golpe pesado
// SFX: Combat_Block.wav — bloqueo
// SFX: Combat_Dodge.wav — esquive
// SFX: Combat_Death.wav — muerte de enemigo genérico
