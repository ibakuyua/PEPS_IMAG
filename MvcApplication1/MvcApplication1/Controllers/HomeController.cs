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
            //System.IO.File.WriteAllLines(@"C:\Users\ayuta\Desktop\Cours_3A_Imag\Peps\ProjetEvaluationProduitStructure21\data\json.txt", CSVtoJSON());
            string result = "[" + String.Join(",",CSVtoJSON() ) + "]";
            //System.IO.File.WriteAllText(@"C:\Users\ayuta\Desktop\Cours_3A_Imag\Peps\ProjetEvaluationProduitStructure21\data\json.txt", result);
            return Content(result, "application/json");
        }

        #region Private methods

        string[] CSVtoJSON()
        {
            string[] allLines = System.IO.File.ReadAllLines(@"C:\Users\ayuta\Desktop\Cours_3A_Imag\Peps\ProjetEvaluationProduitStructure21\data\dataPEPS.csv");
            var initial = new DateTime(1970, 1, 1);
            List<string> listData = new List<string>();
            int ligne;
            for (ligne = 1; ligne < allLines.Length; ligne++)
            {
                string[] data = allLines[ligne].Split(',');
                string[] date = data[0].Split('/');
                var dateFormate = new DateTime(Int32.Parse(date[2]), Int32.Parse(date[1]), Int32.Parse(date[0]));
                listData.Add( "[" + (dateFormate - initial).TotalMilliseconds.ToString() + "," + data[1] + "]");
            }

            return listData.ToArray();
        }


        #endregion

    }
    
}
