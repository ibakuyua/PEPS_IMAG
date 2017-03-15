using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.IO;
using System.Globalization;
using System.Net;

namespace MvcApplication1.Controllers
{
    public class HomeController : Controller
    {
        public ActionResult Index()
        {
            ViewBag.Message = "Pricer Multimonde.";

            return View();
        }

        public ActionResult About()
        {
            ViewBag.Message = "Description du produit Multimonde.";

            return View();
        }

        public ActionResult Historique()
        {
            ViewBag.Message = "Historique de performance des indices.";

            return View();
        }

        public ActionResult Couverture()
        {
            ViewBag.Message = "Composition du portefeuille de couverture.";

            return View();
        }

        public ActionResult Backtest()
        {
            ViewBag.Message = "Backtesting";

            return View();
        }

        /// <summary>
        /// Get serie in JSON format.
        /// </summary>
        /// <returns>JSON quote values.</returns>
        /// <remarks>
        /// The DateTime must be converted in Javascript date. This corresponds to total milliseconds between 01/01/1970 and current date.
        /// </remarks>
        public ActionResult GetSerie()
        {
            // get quote list
            //var quoteList = ConvertFile();

            // convert to JSON foramt : [ [ date1, value1 ], [ date2, value2 ], ... ]
            //var initial = new DateTime(1970, 1, 1);
            //var dataList = quoteList.Select(d => $"[{(d.Key.Date - initial).TotalMilliseconds},{d.Value.ToString(CultureInfo.InvariantCulture).Replace(",", ".")}]").ToList();
            //var result = "[" + string.Join(",", dataList) + "]";
            //result = "[" + initial.ToString() + ", 8]";


            string[] allLines = System.IO.File.ReadAllLines(@"C:\Users\ayuta\Desktop\Cours_3A_Imag\Peps\ProjetEvaluationProduitStructure21\data\dataPEPS.csv");
            var initial = new DateTime(2010, 10, 1);
            string contenu = "";
            int ligne;
            string separator = "";
            for (ligne = 1; ligne < allLines.Length; ligne++ )
            {
                string[] data = allLines[ligne].Split(',');
                string[] date = data[0].Split('/');
                var dateFormate = new DateTime(Int32.Parse(date[2]),Int32.Parse(date[1]),Int32.Parse(date[0]));
                contenu = contenu + separator + "[" + (dateFormate - initial).TotalMilliseconds.ToString() + "," + data[1] + "]";
                separator = ",";
            }
            var result = "[" + string.Join(",",contenu) + "]";
            return Content(result, "application/json");
        }

        #region Private methods

        /// <summary>
        /// Get the URL to download CAC 40 stock.
        /// </summary>
        /// <param name="startDate">Start date.</param>
        /// <returns>URL.</returns>
        private static string GetUrl(DateTime startDate)
        {
            var result = "http://real-chart.finance.yahoo.com/table.csv?s=^FCHI&amp;d=#END_MONTH#&amp;e=#END_DAY#&amp;f=#END_YEAR#&amp;g=d&amp;a=#START_MONTH#&amp;b=#START_DAY#&amp;c=#START_YEAR#&amp;g=d&amp;ignore=.csv";
            result = result.Replace("#START_DAY#", startDate.Date.Day.ToString(CultureInfo.InvariantCulture));
            result = result.Replace("#START_MONTH#", (startDate.Date.Month - 1).ToString(CultureInfo.InvariantCulture));
            result = result.Replace("#START_YEAR#", startDate.Date.Year.ToString(CultureInfo.InvariantCulture));
            result = result.Replace("#END_DAY#", DateTime.Now.Day.ToString(CultureInfo.InvariantCulture));
            result = result.Replace("#END_MONTH#", (DateTime.Now.Month - 1).ToString(CultureInfo.InvariantCulture));
            result = result.Replace("#END_YEAR#", DateTime.Now.Year.ToString(CultureInfo.InvariantCulture));
            return result;
        }

        /// <summary>
        /// Download stock quote file.
        /// </summary>
        /// <param name="url">URL to use.</param>
        /// <returns>Filename.</returns>
        private static string DownloadFile(string url)
        {
            var result = Path.GetTempFileName();
            try
            {
                var client = new WebClient();
                client.DownloadFile(url, result);
            }
            catch (Exception)
            {
                result = string.Empty;
            }
            return result;
        }

        /// <summary>
        /// Convert file lines to dictionary.
        /// </summary>
        /// <returns>Dictionary with date and quote value.</returns>
        private static Dictionary<DateTime, decimal> ConvertFile()
        {
            // get quote file
            var file = DownloadFile(GetUrl(DateTime.Now.AddYears(-1)));

            // convert result
            var result = new Dictionary<DateTime, decimal>();
            if (string.IsNullOrEmpty(file)) return result;

            try
            {
                using (var reader = new StreamReader(file))
                {
                    reader.ReadLine(); // pass header line

                    string line;
                    while (!string.IsNullOrEmpty(line = reader.ReadLine()))
                    {
                        var data = line.Split(',');
                        if (data.Length < 5) continue;

                        // convert values
                        DateTime date;
                        if (!DateTime.TryParseExact(data[0], "yyyy-MM-dd", CultureInfo.InvariantCulture, DateTimeStyles.None, out date)) continue;
                        decimal value;
                        if (!Decimal.TryParse(data[4].Replace('.', ','), out value)) continue;

                        result.Add(date, value);
                    }

                    reader.Close();
                }
            }
            catch { }

            // important : order data ascending date
            return result.OrderBy(x => x.Key).ToDictionary(x => x.Key, y => y.Value);
        }

        #endregion

    }
    
}
