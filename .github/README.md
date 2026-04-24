# GitHub — plantillas y automatización

## Plantillas

| Archivo | Uso |
|---------|-----|
| [`pull_request_template.md`](pull_request_template.md) | Se rellena al abrir un Pull Request en GitHub. |
| [`ISSUE_TEMPLATE/bug_report.md`](ISSUE_TEMPLATE/bug_report.md) | Informe de error. |
| [`ISSUE_TEMPLATE/feature_request.md`](ISSUE_TEMPLATE/feature_request.md) | Propuesta de mejora o feature. |
| [`ISSUE_TEMPLATE/config.yml`](ISSUE_TEMPLATE/config.yml) | Configuración de plantillas (issues en blanco permitidos). |

## Workflows (CI)

La carpeta [`workflows/`](workflows/) está reservada para **GitHub Actions** (cook nocturno,
lint, etc.). Añadid aquí `.yml` cuando el repositorio remoto y los runners estén definidos;
hasta entonces puede permanecer vacía o con un README explicativo.
