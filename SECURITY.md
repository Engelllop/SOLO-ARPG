# Política de seguridad

## Versiones soportadas

| Estado | Versiones / alcance |
|--------|----------------------|
| En desarrollo | Rama(s) activas acordadas por el equipo (p. ej. `main` / `develop`). |

Actualizad esta tabla cuando publiquéis builds etiquetadas (v1.0.0, etc.).

---

## Cómo reportar una vulnerabilidad

**No** abráis un issue público con detalles que permitan explotar el fallo.

1. Enviad un correo a **los mantenedores del proyecto** a la dirección que el equipo configure (sustituid el placeholder cuando lo tengáis):

   **`security@example.com`** ← reemplazar por un correo real del estudio o responsable.

2. Incluid en el mensaje:

   - descripción del problema y impacto posible;
   - pasos para reproducir (si los hay);
   - versión o commit del repositorio;
   - si conocéis mitigación temporal, indicadla por separado.

3. Tiempo de respuesta esperado: **objetivo interno** de 7 días hábiles para un primer ack (ajustad según tamaño del equipo).

---

## Alcance típico en proyectos Unreal

- Cliente / editor: ejecución remota de código, corrupción de guardados, fugas de datos personales.
- Integraciones online (EOS, HTTP): tokens en cliente, validación solo en cliente, endpoints sin autenticación.

---

## Divulgación

Coordinad con los mantenedores antes de publicar writeups o CVE; el objetivo es parchear o mitigar sin poner a los jugadores en riesgo innecesario.
