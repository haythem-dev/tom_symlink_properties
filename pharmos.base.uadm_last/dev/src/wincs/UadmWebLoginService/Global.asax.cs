using Microsoft.Extensions.DependencyInjection;
using System;
using System.Globalization;
using System.Web;
using System.Web.Mvc;
using System.Web.Optimization;
using System.Web.Routing;
using UadmCommon.Classes;
using UadmCommon.Models;
using UadmCommon.Models.Extensions;
using Unity;
using Unity.AspNet.Mvc;
using Unity.Injection;
using Unity.Lifetime;

namespace UadmWebLoginService
{
    public class MvcApplication : System.Web.HttpApplication
    {
        public static ServiceProvider ServiceProvider { get; private set; }

        /// <summary>
        /// Application_BeginRequest
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        protected void Application_BeginRequest(Object sender, EventArgs e)
        {
            SetCultureInfo(HttpContext.Current.Request);
        }

        /// <summary>
        /// Application_Start
        /// </summary>
        protected void Application_Start()
        {
            string appSettingsFileName = "UadmAppSettings.json";

            // Using Microsoft.Extensions.DependencyInjection for singleton registration
            var services = new ServiceCollection();

            // Register UadmAppSettings as singleton
            services.AddSingleton<UadmAppSettings>(provider => UadmAppSettingsExtensions.Instance(appSettingsFileName) ?? new UadmAppSettings(appSettingsFileName));

            ServiceProvider = services.BuildServiceProvider();

            AreaRegistration.RegisterAllAreas();
            FilterConfig.RegisterGlobalFilters(GlobalFilters.Filters);
            RouteConfig.RegisterRoutes(RouteTable.Routes);
            BundleConfig.RegisterBundles(BundleTable.Bundles);

            var container = new UnityContainer();

            // Register types
            container.RegisterType<UadmAppSettings>(new InjectionConstructor(appSettingsFileName));

            // Register UadmAppSettings as a singleton
            container.RegisterType<UadmAppSettings>(
                new ContainerControlledLifetimeManager(),
                new InjectionConstructor(appSettingsFileName)
            );

            UadmLogger.LogInfo ("Application_Start: UadmAppSettings loaded from " + appSettingsFileName);   

            DependencyResolver.SetResolver(new UnityDependencyResolver(container));
        }

        /// <summary>
        /// SetCultureInfo
        /// </summary>
        /// <param name="httpRequest"></param>
        /// <returns></returns>
        public static void SetCultureInfo(HttpRequest httpRequest)
        {
            string[] userLanguages = httpRequest.UserLanguages;
            
            if (userLanguages != null && userLanguages.Length > 0)
            {
                SetCultureInfo(userLanguages[0]);
            }
            else
                SetCultureInfo();
        }

        /// <summary>
        /// SetCultureInfo
        /// </summary>
        /// <param name="countryInfo"></param>
        /// <returns></returns>
        public static void SetCultureInfo(string countryInfo = null)
        {
            string culture = null;

            if (!string.IsNullOrEmpty(countryInfo))
            {
                try
                {
                    culture = CultureInfo.GetCultureInfo(countryInfo).Name;
                }
                catch (CultureNotFoundException)
                {                  
                }
            }

            if (string.IsNullOrEmpty(culture))
            {
                culture = "en-US";
            }

            System.Threading.Thread.CurrentThread.CurrentCulture = new CultureInfo(culture);
            System.Threading.Thread.CurrentThread.CurrentUICulture = new CultureInfo(culture);
        }

        /// <summary>
        /// SetCultureInfoByLanguage
        /// </summary>
        /// <param name="language"></param>
        public static void SetCultureInfoByLanguage(string language)
        {
            string lanuage = "en-US";
            if (!string.IsNullOrEmpty(language))
            {
                switch (language.ToLower())
                {
                    case "en":
                        lanuage = "en-US";
                        break;
                    case "de":
                        lanuage = "de-DE";
                        break;
                    case "fr":
                        lanuage = "fr-FR";
                        break;
                    case "bg":
                        lanuage = "bg-BG";
                        break;
                    case "hr":
                        lanuage = "hr-HR";
                        break;
                    default:
                        lanuage = "en-US";
                        break;
                }
            }

            SetCultureInfo(lanuage);
        }
    }
}
