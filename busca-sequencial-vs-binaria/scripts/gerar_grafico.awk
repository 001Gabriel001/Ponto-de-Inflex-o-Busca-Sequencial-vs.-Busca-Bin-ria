# Gera grafico SVG a partir de dados/resultados.csv (CSV separado por ';').
# O eixo vertical logaritmico evidencia as duas curvas em escalas distintas.
BEGIN {
    FS = ";"
    left = 100
    top = 104
    width = 745
    height = 342
    max_log = 4
    log10 = log(10)
}
NR > 1 && NF >= 3 {
    size[++count] = $1 + 0
    sequential[count] = $2 + 0
    binary[count] = $3 + 0
}
function ypos(value) {
    return top + height - (log(value) / log10) * (height / max_log)
}
function xpos(idx) {
    if (count == 1) return left + width / 2
    return left + (idx - 1) * width / (count - 1)
}
END {
    print "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"960\" height=\"560\" viewBox=\"0 0 960 560\" role=\"img\" aria-labelledby=\"titulo descricao\">"
    print "<title id=\"titulo\">Busca sequencial versus busca binaria</title>"
    print "<desc id=\"descricao\">Grafico com eixo vertical logaritmico de comparacoes para seis tamanhos de vetor. A busca sequencial aumenta de 10 a 5000; a binaria de 4 a 13.</desc>"
    print "<rect width=\"960\" height=\"560\" rx=\"16\" fill=\"#ffffff\"/>"
    print "<text x=\"100\" y=\"48\" font-family=\"Arial,sans-serif\" font-size=\"24\" font-weight=\"700\" fill=\"#111827\">Busca Sequencial vs. Binaria</text>"
    print "<text x=\"100\" y=\"75\" font-family=\"Arial,sans-serif\" font-size=\"14\" fill=\"#475569\">Pior caso | comparacoes logicas (eixo vertical em escala logaritmica)</text>"

    for (k = 0; k <= 4; ++k) {
        y = top + height - k * height / 4
        tick = 1
        for (j = 0; j < k; ++j) tick *= 10
        printf "<line x1=\"%.0f\" y1=\"%.1f\" x2=\"%.0f\" y2=\"%.1f\" stroke=\"#e5e7eb\" stroke-width=\"1\"/>\n", left, y, left + width, y
        printf "<text x=\"%.0f\" y=\"%.1f\" text-anchor=\"end\" font-family=\"Arial,sans-serif\" font-size=\"13\" fill=\"#64748b\">%d</text>\n", left - 15, y + 4, tick
    }
    printf "<line x1=\"%.0f\" y1=\"%.0f\" x2=\"%.0f\" y2=\"%.0f\" stroke=\"#64748b\"/>\n", left, top, left, top + height
    printf "<line x1=\"%.0f\" y1=\"%.0f\" x2=\"%.0f\" y2=\"%.0f\" stroke=\"#64748b\"/>\n", left, top + height, left + width, top + height

    for (i = 1; i <= count; ++i) {
        printf "<text x=\"%.1f\" y=\"%.0f\" text-anchor=\"middle\" font-family=\"Arial,sans-serif\" font-size=\"13\" fill=\"#475569\">%d</text>\n", xpos(i), top + height + 25, size[i]
    }
    printf "<text x=\"%.1f\" y=\"%.0f\" text-anchor=\"middle\" font-family=\"Arial,sans-serif\" font-size=\"14\" fill=\"#111827\">Tamanho do vetor (n)</text>\n", left + width / 2, top + height + 57

    printf "<polyline fill=\"none\" stroke=\"#dc2626\" stroke-width=\"3.5\" stroke-linecap=\"round\" stroke-linejoin=\"round\" points=\""
    for (i = 1; i <= count; ++i) printf "%.1f,%.1f ", xpos(i), ypos(sequential[i])
    print "\"/>"
    printf "<polyline fill=\"none\" stroke=\"#2563eb\" stroke-width=\"3.5\" stroke-linecap=\"round\" stroke-linejoin=\"round\" points=\""
    for (i = 1; i <= count; ++i) printf "%.1f,%.1f ", xpos(i), ypos(binary[i])
    print "\"/>"

    for (i = 1; i <= count; ++i) {
        printf "<circle cx=\"%.1f\" cy=\"%.1f\" r=\"5\" fill=\"#dc2626\" stroke=\"white\" stroke-width=\"1.5\"/>\n", xpos(i), ypos(sequential[i])
        printf "<circle cx=\"%.1f\" cy=\"%.1f\" r=\"5\" fill=\"#2563eb\" stroke=\"white\" stroke-width=\"1.5\"/>\n", xpos(i), ypos(binary[i])
    }
    print "<line x1=\"550\" y1=\"43\" x2=\"582\" y2=\"43\" stroke=\"#dc2626\" stroke-width=\"4\"/>"
    print "<text x=\"592\" y=\"48\" font-family=\"Arial,sans-serif\" font-size=\"14\" fill=\"#334155\">Sequencial</text>"
    print "<line x1=\"704\" y1=\"43\" x2=\"736\" y2=\"43\" stroke=\"#2563eb\" stroke-width=\"4\"/>"
    print "<text x=\"746\" y=\"48\" font-family=\"Arial,sans-serif\" font-size=\"14\" fill=\"#334155\">Binaria</text>"
    if (count > 0) {
        printf "<text x=\"%.1f\" y=\"%.1f\" font-family=\"Arial,sans-serif\" font-size=\"14\" font-weight=\"700\" fill=\"#dc2626\">%d</text>\n", xpos(count) + 13, ypos(sequential[count]) + 4, sequential[count]
        printf "<text x=\"%.1f\" y=\"%.1f\" font-family=\"Arial,sans-serif\" font-size=\"14\" font-weight=\"700\" fill=\"#2563eb\">%d</text>\n", xpos(count) + 13, ypos(binary[count]) + 4, binary[count]
    }
    print "<text x=\"100\" y=\"534\" font-family=\"Arial,sans-serif\" font-size=\"12\" fill=\"#64748b\">Fonte: dados/resultados.csv, gerado pela implementacao em C.</text>"
    print "</svg>"
}
