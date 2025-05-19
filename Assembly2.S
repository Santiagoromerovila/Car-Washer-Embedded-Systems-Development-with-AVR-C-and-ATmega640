; Asumimos:
; M3_DIR_PIN = 0
; M3_EN_PIN  = 2
; M4_EN_PIN  = 6
; M4_DIR_PIN = 4
; M5_DIR_PIN = 1
; PORTL      = 0x010B
; OCR2A      = 0x00B3

.global rodillo_subir
.global rodillo_bajar
.global rodillo_girar
.global rodillo_parar_giro
.global rodillo_detener


; --- Subir rodillo: DIR = 1, EN = 1 ---
rodillo_subir:
    lds  r16, 0x010B
    ori  r16, (1 << 0)         ; M3_DIR_PIN
    ori  r16, (1 << 2)         ; M3_EN_PIN
    sts  0x010B, r16
    ret

; --- Bajar rodillo: DIR = 0, EN = 1 ---
rodillo_bajar:
    lds  r16, 0x010B
    andi r16, 0xFE             ; Borrar bit 0 (M3_DIR_PIN)
    ori  r16, (1 << 2)         ; Establecer M3_EN_PIN
    sts  0x010B, r16
    ret

; --- Girar rodillo: M4_EN = 1, M4_DIR = 0 ---
rodillo_girar:
    lds  r16, 0x010B
    ori  r16, (1 << 6)         ; Establecer M4_EN_PIN (bit 6)
    andi r16, 0xEF             ; Borrar bit 4 (M4_DIR_PIN)
    sts  0x010B, r16
    ret

; --- Parar giro rodillo: M4_EN = 0 ---
rodillo_parar_giro:
    lds  r16, 0x010B
    andi r16, 0xBF             ; Borrar bit 6 (M4_EN_PIN)
    sts  0x010B, r16
    ret

; --- Detener rodillo: M3_EN = 0 ---
rodillo_detener:
    lds  r16, 0x010B
    andi r16, 0xFB             ; Borrar bit 2 (M3_EN_PIN)
    sts  0x010B, r16
    ret

