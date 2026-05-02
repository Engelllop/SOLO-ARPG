// NG+ Mode — hooks y especificaciones
// Integración con SOLOGameInstance (StoryFlags) y GameMode.

// ============================
// ¿Qué cambia en NG+?
// ============================

// 1. NPC Dialogue Changes
//    En NG+, varios NPCs tienen líneas diferentes que ROMAN LA CUARTA PARED.
//    Ejemplo:
//      - Garrik (N3): "Otra vez tú. Otra vez yo. Supongo que el guión lo exige."
//      - Criada Bronce (N8): "Ya me mataste una vez. ¿O fue la siguiente?"
//      - Arquitecto (N10): "NG+ detectado. ¿Esperabas que algo cambiara?"
//
//    Estos diálogos se activan vía StoryFlag "NewGamePlus_Active" que se
//    establece al iniciar NG+.

// 2. Enemy Scaling
//    - Todos los jefes tienen 1.5x HP
//    - Todos los jefes tienen 1.3x daño
//    - No se aplica a minions normales (solo bosses)

// 3. Reward Multiplier
//    - Gold drops: 2x
//    - XP gains: 1.5x
//    - Mejora chance de loot raro

// 4. Retained Items
//    Al iniciar NG+, se conservan:
//    - Todas las habilidades desbloqueadas
//    - Todos los Fragmentos (para Ending B tracking)
//    - NG+ Count (+1)
//    - NO se conservan:
//      - Items de consumo
//      - Oro
//      - Equipamiento no-único

// 5. New Ending Variants
//    Si se completa NG+ en Ending B:
//      - El Arquitecto dice: "Segunda vez. Ojalá pudiera decir que es diferente."
//      - Jugador puede elegir PUBLICAR los archivos (dispara roll de créditos con fotos reales)
//           o QUEMARLOS (créditos con ruido blanco)

// ============================
// CÓDIGO DE REFERENCIA (SOLOGameInstance)
// ============================

// En SOLOGameInstance.h / .cpp (YA EXISTE), agregar:
//
// UFUNCTION(BlueprintCallable, Category = "NG+")
// bool IsNewGamePlus() const;
//
// Implementación:
//   return HasStoryFlag("NewGamePlus_Active");
//
// Al iniciar NG+ (desde el menu/Final):
//   SetStoryFlag("NewGamePlus_Active");
//   USOLOSaveSubsystem* Save = GetSubsystem();
//   if (Save) Save->StartNewGamePlus();

// ============================
// ENEMY SCALING (en SOLOEnemyCharacter)
// ============================

// En SOLOEnemyCharacter.h (YA EXISTE), verificar si ya hay funk:
//   virtual float GetDamageScale() const;
//   virtual float GetHPScale() const;
//
// Si existe, implementar:
//   if (HasStoryFlag("NewGamePlus_Active"))
//       return 1.5f;
//   return 1.0f;
//
// Si no existe, agregar al BeginPlay de cada boss:
//   UGameInstance* GI = GetGameInstance();
//   if (GI && GI->GetSubsystem<USOLOGameInstance>())
//       bIsNGP = GI->GetSubsystem<USOLOGameInstance>()->HasStoryFlag("NewGamePlus_Active");
//   if (bIsNGP) { MaxHP *= 1.5f; }

// ============================
// NG+ Dialogue Integration
// ============================

// Cada NPC que debe tener diálogo NG+ debe:
// 1. Al BeginPlay, checkear:
//      UGameInstance* GI = GetGameInstance();
//      if (GI && Cast<USOLOGameInstance>(GI)->HasStoryFlag("NewGamePlus_Active"))
//          DialogueNode = "N3_Garrik_NGP_Intro" (en vez del normal)
// 2. Agregar el nodo NG+ a los data tables correspondientes
// 3. prefijo "_NGP" en los Node IDs

// ============================
// Endings en NG+
// ============================

// Ending B NG+:
//   En N10, si NG+ activo:
//     Arquitecto: "¿De verdad crees que esta vez será diferente?"
//     Pantalla: "PUBLICAR / QUEMAR"
//     PUBLICAR: créditos con fotos de víctimas reales (placeholder: texto blanco sobre negro)
//     QUEMAR: créditos en rojo, texto: "Nunca pasó."
//
//   Después de créditos NG+:
//     Si PUBLICAR:
//       Noticia: "Documentos filtrados generan investigación global."
//       Jugador recibe logro: "La Verdad"
//     Si QUEMAR:
//       Noticia: "Incendio en archivo central. No hay sobrevivientes."
//       Jugador recibe logro: "El Silencio"
