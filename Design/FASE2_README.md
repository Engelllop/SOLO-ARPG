# FASE2.enc

Contenido de diseño de fases futuras del juego, cifrado con AES-256-CBC (PBKDF2, 200k iteraciones). La clave NO está en este repo.

**Descifrar** (Git Bash, desde la raíz del repo):

```bash
openssl enc -d -aes-256-cbc -pbkdf2 -iter 200000 -in Design/FASE2.enc -pass pass:"LA_CLAVE" | tar -xz
```

Extrae los `.md` en el directorio actual. Para trabajar en ellos, extraer FUERA del repo o en una carpeta ignorada, editar, y re-cifrar:

```bash
tar -czf - *.md | openssl enc -aes-256-cbc -pbkdf2 -iter 200000 -salt -out Design/FASE2.enc -pass pass:"LA_CLAVE"
```

**Nunca commitear los `.md` extraídos.**
