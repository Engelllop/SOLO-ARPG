# GuÃ­a de contribuciÃ³n â€” SOLO

Objetivo: que **cualquier persona** (humana o asistente) pueda contribuir **sin romper convenciones**, **sin filtrar secretos** y **dejando rastro** en documentaciÃ³n y changelog cuando toque.

---

## 1. Antes de tocar el repositorio

Leed en este orden (o confirmad que ya estÃ¡ asimilado):

1. [`README.md`](README.md) â€” estructura del disco, mÃ³dulos, plugins, roadmap resumido.
2. [`Docs/ONBOARDING.md`](Docs/ONBOARDING.md) â€” a quÃ© carpeta ir, trampas habituales.
3. [`Design/GDD/GAME_DESIGN_DOCUMENT.md`](Design/GDD/GAME_DESIGN_DOCUMENT.md) â€” diseÃ±o de producto.
4. [`Docs/ROADMAP_FASES.md`](Docs/ROADMAP_FASES.md) â€” prioridad FASE 0 / 1 / 2.
5. [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md) â€” conducta esperada.
6. Si usÃ¡is **asistentes de IA** en el desarrollo: [`AGENTS.md`](AGENTS.md).

---

## 2. Flujo de trabajo con Git

ConvenciÃ³n recomendada (ajustad nombres si usÃ¡is otro modelo):

| Rama | Uso |
|------|-----|
| `main` | Historial estable; solo entra cÃ³digo revisado y compilable segÃºn criterio del equipo. |
| `develop` | (Opcional) IntegraciÃ³n diaria antes de fusionar a `main`. |
| `feature/<nombre-corto>` | Una rama por tarea o ticket: `feature/tower-floor-save`. |

**Commits:** mensajes claros en espaÃ±ol o inglÃ©s, pero **consistentes en el repo**. Se recomienda una primera lÃ­nea corta (`feat: inventario stack`) y cuerpo opcional con contexto.

**FusiÃ³n:** preferid *merge* o *squash* segÃºn polÃ­tica del equipo; documentad la decisiÃ³n aquÃ­ en una lÃ­nea cuando la fijÃ©is: *(pendiente de acordar)*.

---

## 3. CÃ³digo C++ (Unreal)

- **Gameplay nuevo:** prioridad del mÃ³dulo **`SOLOCore`**, salvo que la tarea sea explÃ­citamente ensamblaje en **`SOLO`**.
- **Nombres:** prefijo **`SOLO`** en tipos propios; widgets UMG en C++ con prefijo **`WG_`** donde ya se usa en el proyecto.
- **Build:** tras cambiar `*.Build.cs` o dependencias de mÃ³dulo, regenerad proyectos de IDE si hace falta y compilad el target del editor.
- **Sin secretos:** no commitear claves API, tokens EOS, certificados ni `User*.ini` con datos sensibles (ver `.gitignore`).

---

## 4. ConfiguraciÃ³n, datos y balance

- JSON y balance bajo `Config/` â€” cambios que afecten gameplay: entrada en [`Docs/Changelog/CHANGELOG.md`](Docs/Changelog/CHANGELOG.md) cuando el resto del equipo deba enterarse.
- Nuevos formatos de datos: documentad el esquema en `Docs/Systems/` o en el GDD, y enlazad desde el Ã­ndice de sistemas.

---

## 5. Contenido (`Content/`) y `Assets/`

- **`Assets/`:** arte y referencias â€œfueraâ€ del flujo tÃ­pico de `.uasset` (ver [`Assets/README.md`](Assets/README.md)).
- **`Content/`:** assets del editor; notas en [`Content/README.md`](Content/README.md). Decidid en equipo si se versiona (lÃ­nea comentada en `.gitignore`). Los binarios grandes suelen ir a **Git LFS** o a almacÃ©n externo; si adoptÃ¡is LFS, documentad comandos en este archivo.

---

## 6. DocumentaciÃ³n obligatoria por tipo de cambio

| Tipo de cambio | QuÃ© actualizar |
|----------------|----------------|
| Nuevo sistema o flujo tÃ©cnico | GuÃ­a en `Docs/Systems/` + fila en su [`README.md`](Docs/Systems/README.md). |
| Cambio de prioridad de producto | [`Docs/ROADMAP_FASES.md`](Docs/ROADMAP_FASES.md) y, si aplica, tabla del README raÃ­z. |
| Cambio visible para diseÃ±o / QA | [`Docs/Changelog/CHANGELOG.md`](Docs/Changelog/CHANGELOG.md). |
| Nuevo riesgo legal o dependencia | [`Docs/LEGAL_NOTICES.md`](Docs/LEGAL_NOTICES.md). |

---

## 7. Pull requests

Usad la plantilla en [`.github/pull_request_template.md`](.github/pull_request_template.md) (GitHub la rellena sola al abrir un PR).

**Checklist mÃ­nima del revisor**

- Compila el target relevante (Editor / Game).
- No hay secretos ni rutas absolutas personales en el diff.
- DocumentaciÃ³n y changelog alineados con la secciÃ³n 6.

---

## 8. Seguridad

Si encontrÃ¡is una vulnerabilidad, **no** abrÃ¡is issue pÃºblico con el exploit. Seguid [`SECURITY.md`](SECURITY.md).

---

## 9. Licencia y titularidad

- El texto base de licencia estÃ¡ en [`LICENSE`](LICENSE).
- Sustituid la frase de titularidad si vuestro nombre legal o estudio comercial es otro, **tras** revisiÃ³n interna o legal si aplica.

---

## 10. Glosario rÃ¡pido

Ver [`Docs/GLOSSARIO.md`](Docs/GLOSSARIO.md).

---

*Ãšltima revisiÃ³n: abril 2026.*
