using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
//using Wrapper;
using MvcApplication1.Models;
using System.IO;
using System.Globalization;
using System.Net;


namespace MvcApplication1.Controllers
{
    public class HomeController : Controller
    {

        Dictionary<string, int> stocks = new Dictionary<string, int>()
        {
            {"EuroStoxx500",1},
            {"FTSE100",2},
            {"SP500",3},
            {"HangSeng",4},
            {"Nikkei225",5},
            {"SPASX200",6},
            {"GBPEUR",7},
            {"USDEUR",8},
            {"HKDEUR",9},
            {"JPYEUR",10},
            {"AUDEUR",11}
        };

        public ActionResult Index(int id = 0)
        {
            /*ViewBag.Message = "Pricer Multimonde.";
            double prixMultimonde = 0;
            Price priceM2021 = new Price();
            if (id == 1)
            {
                prixMultimonde = priceM2021.prix;
                //priceM2021.test();
            }
            ViewData.Add("prixMultimonde", prixMultimonde);*/
            return View();
        }

        private void goBtn_Click()
        {
            // Retrieve the values of the parameters in the TextBoxes
           /* Price priceM2021 = new Price();
            double prixM2021 = priceM2021.prix;*/
            
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

        public ActionResult Change()
        {
            ViewBag.Message = "Historique de performance des changes.";

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
        public ActionResult GetSerie(string name)
        {
            int index = stocks[name];
            //System.IO.File.WriteAllLines(@"C:\Users\ayuta\Desktop\Cours_3A_Imag\Peps\ProjetEvaluationProduitStructure21\data\json.txt", CSVtoJSON());
            string result = "[" + String.Join(",",CSVtoJSON(index) ) + "]";
            //System.IO.File.WriteAllText(@"C:\Users\ayuta\Desktop\Cours_3A_Imag\Peps\ProjetEvaluationProduitStructure21\data\json.txt", result);
            return Content(result, "application/json");
        }

        #region Private methods

        string[] CSVtoJSON(int underlyer)
        {
            //METTRE le chemin absolu du fichier
            string[] allLines = System.IO.File.ReadAllLines(@"C:\Users\ayuta\Desktop\Cours_3A_Imag\Peps\ProjetEvaluationProduitStructure21\data\dataPEPS.csv");
            var initial = new DateTime(1970, 1, 1);
            List<string> listData = new List<string>();
            int ligne;
            for (ligne = 1; ligne < 450; ligne++)
            {
                string[] data = allLines[ligne].Split(',');
                string[] date = data[0].Split('/');
                var dateFormate = new DateTime(Int32.Parse(date[2]), Int32.Parse(date[1]), Int32.Parse(date[0]));
                listData.Add( "[" + (dateFormate - initial).TotalMilliseconds.ToString() + "," + data[underlyer] + "]");
            }

            return listData.ToArray();
        }


        #endregion

    }
    
}
