#!/bin/sh
# Testa escolhas validas, alvo presente, ausente e entrada incorreta.
set -eu
saida="$(mktemp)"
trap 'rm -f "$saida"' EXIT HUP INT TERM

printf '1\n12\n' | ./bin/comparador > "$saida"
grep -Fq 'Vetor: 10 elementos' "$saida"
grep -Fq 'Valor procurado: 12' "$saida"
grep -Fq '  Comparacoes: 7' "$saida"
grep -Fq '  Comparacoes: 4' "$saida"

printf '10\n21\n' | ./bin/comparador > "$saida"
grep -Fq 'Numero nao encontrado.' "$saida"
grep -Fq '  Comparacoes: 10' "$saida"
grep -Fq '  Comparacoes: 4' "$saida"

printf '6\n9999\n' | ./bin/comparador > "$saida"
grep -Fq 'Vetor: 5000 elementos' "$saida"
grep -Fq '  Comparacoes: 5000' "$saida"
grep -Fq '  Comparacoes: 13' "$saida"

printf 'erro\n3\ninvalid\n-4\n' | ./bin/comparador > "$saida"
grep -Fq 'Entrada invalida.' "$saida"
grep -Fq 'Numero invalido.' "$saida"
grep -Fq 'Valor procurado: -4' "$saida"

printf '%s\n' 'OK: menu interativo testado com sucesso.'
