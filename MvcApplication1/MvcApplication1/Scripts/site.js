function chart_addSerie(chart, data) {
    /// <summary>
    /// Add a serie to a chart.
    /// </summary>
    /// <param name="chart">Chart.</param>
    /// <param name="data">Serie list.</param>
    "use strict";

    // add serie
    var serie = { name: 'CAC 40', data: data };
    serie.marker = {
        enabled: true,
        radius: 2
    };
    chart.addSeries(serie);
}

function chartInitialize(id, url) {
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
        rangeSelector: { selected: 1 },
        series: null,

        yAxis: [
            { title: { text: "Ma série" } },
            {
                gridLineWidth: 0,
                plotLines: [{ color: '#FF0000', width: 2, value: 0 }]
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
            chart_addSerie(Highcharts.charts[0], result);
        },
        "json");
}