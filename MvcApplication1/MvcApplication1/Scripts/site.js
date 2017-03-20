function chart_addSerie(chart, data, name) {
    /// <summary>
    /// Add a serie to a chart.
    /// </summary>
    /// <param name="chart">Chart.</param>
    /// <param name="data">Serie list.</param>
    "use strict";

    // add serie
    var serie = { name: name, data: data };
    serie.marker = {
        enabled: true,
        radius: 2
    };
    chart.addSeries(serie);
}

function chartInitialize(name,id, url) {
    /// <summary>
    /// Initialize a chart in french language.
    /// </summary>
    /// <param name="id">Div identifier.</param>
    /// <param name="url">URL to call to get serie.</param>
    Highcharts.setOptions({
        lang: {
            months: ['Janvier', 'Février', 'Mars', 'Avril', 'Mai', 'Juin', 'Juillet', 'Août', 'Septembre', 'Octobre', 'Novembre', 'Décembre'],
            weekdays: ['Dimanche', 'Lundi', 'Mardi', 'Mercredi', 'Jeudi', 'Vendredi', 'Samedi'],
            decimalPoint: ',',
            thousandsSep: ''
        },

        credits: {
            enabled: false
        }
    });

    // initialize chart
    $("#" + id).highcharts('StockChart', {
        rangeSelector: { selected: 5 },
        series: null,

        yAxis: [
            { title: { text: "Cours de l'indice" } },
            {
                gridLineWidth: 0,
                plotLines: [{ color: '#FF0000', width: 2, value: 0 }, { color: '#00FF00', width: 2, value: 0 }]
            },
        ],
        tooltip: {
            crosshairs: { color: 'blue', dashStyle: 'solid' },
            shared: true
        }
    });

    // get and display data
    $.get(url,
        null,
        function (result) {
            chart_addSerie(Highcharts.charts[0], result,name);
        },
        "json");
}

function chartUpdate(name, id, url) {
    Highcharts.charts[0].series[0].remove(true);
    $.get(url,
        null,
        function (result) {
            chart_addSerie(Highcharts.charts[0], result, name);
        },
        "json");

}
