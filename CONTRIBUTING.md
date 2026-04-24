# Guía de contribución — EMBERVEIL

Objetivo: que **cualquier persona** (humana o asistente) pueda contribuir **sin romper convenciones**, **sin filtrar secretos** y **dejando rastro** en documentación y changelog cuando toque.

---

## 1. Antes de tocar el repositorio

Leed en este orden (o confirmad que ya está asimilado):

1. [`README.md`](README.md) — estructura del disco, módulos, plugins, roadmap resumido.
2. [`Docs/ONBOARDING.md`](Docs/ONBOARDING.md) — a qué carpeta ir, trampas habituales.
3. [`Design/GDD/GAME_DESIGN_DOCUMENT.md`](Design/GDD/GAME_DESIGN_DOCUMENT.md) — diseño de producto.
4. [`Docs/ROADMAP_FASES.md`](Docs/ROADMAP_FASES.md) — prioridad FASE 0 / 1 / 2.
5. [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md) — conducta esperada.
6. Si usáis **asistentes de IA** en el desarrollo: [`AGENTS.md`](AGENTS.md).

---

## 2. Flujo de trabajo con Git

Convención recomendada (ajustad nombres si usáis otro modelo):

| Rama | Uso |
|------|-----|
| `main` | Historial estable; solo entra código revisado y compilable según criterio del equipo. |
| `develop` | (Opcional) Integración diaria antes de fusionar a `main`. |
| `feature/<nombre-corto>` | Una rama por tarea o ticket: `feature/tower-floor-save`. |

**Commits:** mensajes claros en español o inglés, pero **consistentes en el repo**. Se recomienda una primera línea corta (`feat: inventario stack`) y cuerpo opcional con contexto.

**Fusión:** preferid *merge* o *squash* según política del equipo; documentad la decisión aquí en una línea cuando la fijéis: *(pendiente de acordar)*.

---

## 3. Código C++ (Unreal)

- **Gameplay nuevo:** prioridad del módulo **`EMBERVEILCore`**, salvo que la tarea sea explícitamente ensamblaje en **`EMBERVEIL`**.
- **Nombres:** prefijo **`EMBERVEIL`** en tipos propios; widgets UMG en C++ con prefijo **`WG_`** donde ya se usa en el proyecto.
- **Build:** tras cambiar `*.Build.cs` o dependencias de módulo, regenerad proyectos de IDE si hace falta y compilad el target del editor.
- **Sin secretos:** no commitear claves API, tokens EOS, certificados ni `User*.ini` con datos sensibles (ver `.gitignore`).

---

## 4. Configuración, datos y balance

- JSON y balance bajo `Config/` — cambios que afecten gameplay: entrada en [`Docs/Changelog/CHANGELOG.md`](Docs/Changelog/CHANGELOG.md) cuando el resto del equipo deba enterarse.
- Nuevos formatos de datos: documentad el esquema en `Docs/Systems/` o en el GDD, y enlazad desde el índice de sistemas.

---

## 5. Contenido (`Content/`) y `Assets/`

- **`Assets/`:** arte y referencias “fuera” del flujo típico de `.uasset` (ver [`Assets/README.md`](Assets/README.md)).
- **`Content/`:** assets del editor; notas en [`Content/README.md`](Content/README.md). Decidid en equipo si se versiona (línea comentada en `.gitignore`). Los binarios grandes suelen ir a **Git LFS** o a almacén externo; si adoptáis LFS, documentad comandos en este archivo.

---

## 6. Documentación obligatoria por tipo de cambio

| Tipo de cambio | Qué actualizar |
|----------------|----------------|
| Nuevo sistema o flujo técnico | Guía en `Docs/Systems/` + fila en su [`README.md`](Docs/Systems/README.md). |
| Cambio de prioridad de producto | [`Docs/ROADMAP_FASES.md`](Docs/ROADMAP_FASES.md) y, si aplica, tabla del README raíz. |
| Cambio visible para diseño / QA | [`Docs/Changelog/CHANGELOG.md`](Docs/Changelog/CHANGELOG.md). |
| Nuevo riesgo legal o dependencia | [`Docs/LEGAL_NOTICES.md`](Docs/LEGAL_NOTICES.md). |

---

## 7. Pull requests

Usad la plantilla en [`.github/pull_request_template.md`](.github/pull_request_template.md) (GitHub la rellena sola al abrir un PR).

**Checklist mínima del revisor**

- Compila el target relevante (Editor / Game).
- No hay secretos ni rutas absolutas personales en el diff.
- Documentación y changelog alineados con la sección 6.

---

## 8. Seguridad

Si encontráis una vulnerabilidad, **no** abráis issue público con el exploit. Seguid [`SECURITY.md`](SECURITY.md).

---

## 9. Licencia y titularidad

- El texto base de licencia está en [`LICENSE`](LICENSE).
- Sustituid la frase de titularidad si vuestro nombre legal o estudio comercial es otro, **tras** revisión interna o legal si aplica.

---

## 10. Glosario rápido

Ver [`Docs/GLOSSARIO.md`](Docs/GLOSSARIO.md).

---

*Última revisión: abril 2026.*
