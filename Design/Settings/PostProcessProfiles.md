// Post-Process Profiles por Piso — configuración visual
// Aplica via BP_GameMode->SetPostProcess en cada Level Blueprint

// N0 — Caín's Descent
// Tono: Gris, neblina, contraste bajo
// PP: Desaturación 20%, Niebla de altura color gris, Vignette ligero

// N1 — Velmar (Ciudad Colgante)
// Tono: Cálido, dorado, esperanza falsa
// PP: Bloom suave, ColorGrading +0.3 cálido, Saturación +10%

// N2 — Pastor de Cenizas
// Tono: Quemado, naranja/gris, calor opresivo
// PP: Desaturación 30%, tinte naranja, neblina de partículas de ceniza

// --- N3 a N10 ---

// N3 — Aelthar (Pre-revelación)
// Tono: Blanco/dorado brillante, hermoso
// PP: 
//   Bloom Intensity: 1.5
//   ColorGrading: Lift +0.1 blanco
//   SceneColorTint: (1.0, 0.95, 0.85) — cálido
//   AmbientOcclusion: Intensity 0.3
//   AutoExposure: Min 0.5, Max 3.0

// N3 — Aelthar (Post-revelación / Sótano)
// Tono: Oscuro, azul/gris, enfermizo
// PP:
//   Contrast: +0.2
//   SceneColorTint: (0.7, 0.7, 0.8)
//   Vignette: Intensity 0.5
//   Desaturación: 40%

// N4 — Bosque Petrificado
// Tono: Gris total, sin color, niebla
// PP:
//   Saturatión: -100% (gris completo)
//   Contrast: +0.3
//   SceneColorTint: (0.5, 0.5, 0.5)
//   FogDensity: 0.02
//   FogColor: (0.4, 0.4, 0.4)
//   No Bloom
//   No AmbientOcclusion (flat dead look)

// N5 — Thornwall
// Tono: Noche permanente, rojo/anaranjado, luces de burdel
// PP:
//   ColorGrading: Shadows tint rojo (0.8, 0.2, 0.1)
//   SceneColorTint: (0.9, 0.6, 0.4)
//   Vignette: Intensity 0.4
//   Bloom: Intensity 0.5 (solo en fuentes de luz roja)
//   AutoExposure: Min 0.2, Max 1.0 (noche)

// N6 — Cámaras de Jade
// Tono: Verde pulsante, frío, industrial
// PP:
//   SceneColorTint: (0.5, 0.8, 0.5)
//   ColorGrading: Shadows tint verde (0.1, 0.5, 0.1)
//   Bloom: Intensity 0.8 (jade glow)
//   Vignette: Intensity 0.2
//   AmbientCubemap (fake GI verde)

// N7 — Sahal
// Tono: Gris carmesí, carne, enfermizo
// PP:
//   SceneColorTint: (0.7, 0.5, 0.5)
//   Desaturación: -50%
//   Contrast: +0.4 (sombras duras)
//   Vignette: Intensity 0.6 (claustrofóbico)
//   FilmGrain: Intensity 0.2

// N8 — Jardines Flotantes (Exterior)
// Tono: Brillo engañoso, verde/azul natural
// PP:
//   Bloom: Intensity 1.2
//   SceneColorTint: (0.85, 0.9, 0.8)
//   AmbientOcclusion: Intensity 0.2

// N8 — Jardines Flotantes (Interior / Laboratorio)
// Tono: Frío, clínico, blanco/verde
// PP:
//   SceneColorTint: (0.8, 0.9, 0.85)
//   Contrast: +0.2
//   Vignette: 0.3
//   No bloom (ambiente estéril)

// N9 — Argentia
// Tono: Sepia oscura, sombras duras
// PP:
//   Desaturación: -40%
//   SceneColorTint: (0.6, 0.5, 0.3)
//   Contrast: +0.5
//   Vignette: Intensity 0.5 (encajonado)
//   FilmGrain: 0.15
//   AutoExposure: Min 0.15, Max 2.0

// N10 — Corredor de Espejos
// Tono: Blanco/azul, etéreo
// PP:
//   Bloom: Intensity 2.0 (overexposed)
//   SceneColorTint: (0.9, 0.9, 1.0)
//   Vignette: 0.0

// N10 — Sala del Clon
// Tono: Blanco infinito, sin sombras
// PP:
//   Desaturación: -100%
//   SceneColorTint: (1.0, 1.0, 1.0)
//   Exposure: Fijo 0.0 (sin auto)
//   No shadows, no AO, no bloom
//   Fog color: (1.0, 1.0, 1.0)

// N10 — Oficina del Arquitecto
// Tono: Normalidad absoluta, office lighting
// PP:
//   Sin PP override — usar defaults del engine
//   La oficina debe verse REAL comparada al resto de la Torre
