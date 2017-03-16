using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using Wrapper;
using MvcApplication1.Models;



namespace MvcApplication1.Controllers
{
    public class HomeController : Controller
    {

        public ActionResult Index(int id = 0)
        {
            ViewBag.Message = "Pricer Multimonde.";
            double prixMultimonde = 0;
            Price priceM2021 = new Price();
            if (id == 1)
            {
                prixMultimonde = priceM2021.prix;
                //priceM2021.test();
            }
            ViewData.Add("prixMultimonde", prixMultimonde);
            return View();
        }

        private void goBtn_Click()
        {
            // Retrieve the values of the parameters in the TextBoxes
            Price priceM2021 = new Price();
            double prixM2021 = priceM2021.prix;
            
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
    }
}
