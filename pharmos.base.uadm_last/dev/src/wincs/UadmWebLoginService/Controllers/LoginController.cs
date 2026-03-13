using Microsoft.Extensions.DependencyInjection;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using UadmCommon;
using UadmCommon.Classes;
using UadmCommon.Extensions;
using UadmCommon.Models;
using UadmCommon.Models.Extensions;
using UadmWebLoginService.App_GlobalResources;
using UadmWebLoginService.Models;

namespace UadmWebLoginService.Controllers
{
    //[Authorize]
    public class LoginController : Controller
    {
        private readonly UadmAppSettings _uadmAppSettings = null;

        private readonly bool _useAppSettings = true;

        private LibAppUadmWrapper _libAppUadmWrapper = new LibAppUadmWrapper();

        public string RequestId = Guid.NewGuid().ToString().ToLower();

        /// <summary>
        /// LoginController
        /// </summary>
        /// <param name="uadmAppSettings"></param>
        public LoginController(UadmAppSettings uadmAppSettings)
        {
            // get settings from DI container
            _uadmAppSettings = MvcApplication.ServiceProvider.GetService<UadmAppSettings>();
            _libAppUadmWrapper.OsAccount = UadmUserToken.GetWindowsUserId(new HttpContextWrapper(System.Web.HttpContext.Current));

            //_uadmAppSettings = uadmAppSettings;
            //_uadmAppSettings = UadmAppSettings.Instance;
        }

        /// <summary>
        /// Index: analyze request values and show login page
        /// </summary>
        /// <param name="rv">request values</param>
        /// <returns></returns>
        public ActionResult Index(string rv)
        {
            string app = null;
            string environment = null;
            string country = null;
            string language = null;
            string branch = null;

            string originUrl = null;
            string osAccount = ""; // UadmUserToken.GetOSAccount(HttpContext) ?? "johndoe";

            try
            {
                Dictionary<string, string> requestValues = UadmRequestValues.GetRequestValuesFromQueryString(rv);
               
                if (requestValues != null)
                {
                    originUrl = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_ORIGINURL, originUrl);
                    osAccount = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_USERNAME, osAccount);
                    app = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_APPLICATIONKEY, app);
                    environment = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_ENVIRONMENTKEY, environment);
                    country = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_COUNTRY, country);
                    language = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_LANGUAGE, language);
                    branch = UadmRequestValues.GetValueFromRequestValues(requestValues, UadmRequestValues.REQUESTVALUES_PHX_UADM_BRANCHREGION, branch);
                }
            }
            catch (Exception ex)
            {
                UadmLogger.LogError("LoginController.Index: Exception during parsing request headers: " + ex.ToString());
            }

            // check for empty os account and if allowed
            if (string.IsNullOrEmpty(osAccount) && WebConfig.GetUadmAllowEmptyOsAccount(false) == false)
            {
                UadmLogger.LogDebug("LoginController.Index: No OS account found and not allowed.");
                return View("NoUserInformation");
            }

            // check for excluded os accounts   
            if (IsExcludedOSAccount(osAccount))
            {
                return View("ExcludedOsAccount");
            }

            if (string.IsNullOrEmpty(originUrl))
            {
                UadmLogger.LogDebug("LoginController.Index: No application URL cookie found.");
            }

            UadmAppSettingsUrlEnvironment uadmAppUrlEnvironment = _uadmAppSettings.GetUadmAppUrlEnvironment(originUrl);

            if (uadmAppUrlEnvironment != null)
            {
                if (!string.IsNullOrEmpty(uadmAppUrlEnvironment.ApplicationKey))
                    app = uadmAppUrlEnvironment.ApplicationKey;

                if (!string.IsNullOrEmpty(uadmAppUrlEnvironment.Environment))
                    environment = uadmAppUrlEnvironment.Environment;
            }   

            LoginViewModel model = new LoginViewModel 
            { 
                SelectedApplication = app,
                SelectedEnvironment = environment,
                SelectedCountry = country,
                SelectedLanguage = language,
                SelectedApplicationItem = branch,                    
                OriginUrl = originUrl,
                OsAccount = osAccount,
            };

            // load previous login data
            model = GetLoginViewModel(model);

            MvcApplication.SetCultureInfoByLanguage(model.SelectedLanguage);

            return View(model);
        }

        /// <summary>
        /// Index: process login and redirect to application if successful
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        [HttpPost]
        public ActionResult Index(LoginViewModel model)
        {
            MvcApplication.SetCultureInfoByLanguage(model.SelectedLanguage);

            if (!ModelState.IsValid)
            {
                model = GetLoginViewModel(model, false, false);

                SetLoginValidationMessages(model);

                return View(model);
            }
            
            // setup voodoo
            UadmLibEnvironment uadmLibEnvironment = GetLibEnvironment(model);

            string errorMessage = Resources.ErrorLoginFailed;

            try
            {
                // open connection to UADML DLL
                if (_libAppUadmWrapper.OpenConnection(uadmLibEnvironment) == false)
                {
                    model = GetLoginViewModel(model);
                    ModelState.AddModelError("", Resources.ErrorUadmConnection);

                    return View(model);
                }

                UadmLibUser user = _libAppUadmWrapper.VerifyUser(model.Username, model.Password, false);

                // check if user is authenticated
                if ((user != null) && (user.IsAuthenticated == true))
                {
                    // set previous login data
                    SetPreviousLoginData(model, user);

                    // check if user is active
                    if (user.IsActive == true)
                    {
                        // check if password change is required
                        if ((user.IsPasswordChangeRequired == true) && (WebConfig.GetShowPasswordChangeRequired() == true))
                        {
                            model.Password = "";
                            model.NewPassword1 = "";
                            model.NewPassword2 = "";

                            return View("NewPassword", model);
                        }

                        if (HandleAuthenticatedUser(model, user) == false)
                        {
                            return View(model);
                        }

                        UadmLogger.LogInfo("Login RedirectToApplication: " + ViewBag.LoginAuthenticationValidUrl);

                        return RedirectToApplication(model, ViewBag.LoginAuthenticationValidUrl);
                    }
                    else
                    {
                        errorMessage = Resources.ErrorResponseUserIsNotActivated;
                    }
                }
            }
            catch (Exception ex) 
            {
                UadmLogger.LogError("LoginController.Index: Exception during login: " + ex.ToString());
            }
            finally
            {
                _libAppUadmWrapper.CloseConnection();
            }

            // login failed
            model = GetLoginViewModel(model, false, false);

            ModelState.AddModelError("", errorMessage);

            return View(model);
        }

        /// <summary>
        /// NewPasswordCancel
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        [HttpPost]
        public ActionResult NewPasswordCancel(LoginViewModel model)
        {
            try
            {
                bool isChangePasswordRequired = model.IsChangePasswordRequired;

                model = GetLoginViewModel(model);

                ModelState.Clear();

                if (isChangePasswordRequired == true)
                {
                    ModelState.AddModelError("", Resources.ValidationPasswordChangeRequired);
                }

                model.Password = "";    
            }
            catch (Exception ex)
            {
                UadmLogger.LogError("LoginController.Index: Exception during login: " + ex.ToString());
            }
            finally
            {
                _libAppUadmWrapper.CloseConnection();
            }

            return View("Index", model);
        }

        /// <summary>
        /// ChangePassword
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        [HttpPost]
        public ActionResult ChangePassword(LoginViewModel model)
        {
            // Reset state
            ModelState.Clear();
            
            model.IsChangePasswordRequired = false;  

            model.Password = "";
            model.NewPassword1 = "";
            model.NewPassword2 = "";

            return View("NewPassword", model);
        }

        /// <summary>
        /// NewPasswordChange
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        [HttpPost]
        public ActionResult NewPassword(LoginViewModel model)
        {
            MvcApplication.SetCultureInfoByLanguage(model.SelectedLanguage);

            if (model.IsChangePasswordRequired == true)
            {
                // validate new password values
                ActionResult result = CheckNewPasswordValues(model);
                if (result != null)
                {
                    SetNewPasswordValidationMessages(model);

                    return result;
                }
            }

            // save user data to file
            // setup voodoo
            UadmLibEnvironment uadmLibEnvironment = GetLibEnvironment(model);

            string errorMessage = Resources.ErrorLoginFailed; 
            int errorCode = 0;  

            try
            {
                if (_libAppUadmWrapper.OpenConnection(uadmLibEnvironment) == false)
                {
                    model = GetLoginViewModel(model);
                    ModelState.AddModelError("", Resources.ErrorUadmConnection);

                    return View("NewPassword", model);
                }

                UadmLibUser user = _libAppUadmWrapper.ChangeUserPassword(model.Username, model.Password, model.NewPassword1, false, out errorCode, out errorMessage);

                if (user != null)
                {
                    if (user.IsAuthenticated == true)
                    {
                        SetPreviousLoginData(model, user);

                        if (user.IsActive == true)
                        {
                            // set new password for login
                            model.Password = model.NewPassword1;

                            if (HandleAuthenticatedUser(model, user) == false)
                            {
                                MvcApplication.SetCultureInfoByLanguage(model.SelectedLanguage);

                                return View("NewPassword", model);
                            }

                            UadmLogger.LogInfo("NewPassword RedirectToApplication: " + ViewBag.LoginAuthenticationValidUrl);

                            return RedirectToApplication(model, ViewBag.LoginAuthenticationValidUrl);
                        }
                        else
                        {
                            errorMessage = Resources.ErrorResponseUserIsNotActivated;
                            errorCode = (int)UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_USER_IS_NOT_ACTIVATED;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                UadmLogger.LogError("LoginController.Index: Exception during login: " + ex.ToString());
            }
            finally
            {
                _libAppUadmWrapper.CloseConnection();
            }

            UadmAppSettingsEnvironment uadmAppSettingsEnvironment = _uadmAppSettings.GetEnvironment(model.SelectedEnvironment);

            errorMessage = GetErrorMessageFromResponse (errorCode, errorMessage, uadmAppSettingsEnvironment.UseNewPasswordRules);

            // login failed
            model = GetLoginViewModel(model);
            ModelState.AddModelError("", errorMessage);

            model.Password = "";
            model.NewPassword1 = "";
            model.NewPassword2 = "";

            return View(model);
        }

        /// <summary>
        /// CheckNewPasswordValues
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        private ActionResult CheckNewPasswordValues(LoginViewModel model)
        {
            string errorMessage = "";   

            if (!ModelState.IsValid)
            {
                errorMessage = Resources.ErrorFieldsRequired;
            }

            if (string.IsNullOrEmpty(model.Password))
            {
                errorMessage = Resources.ErrorCurrentPasswordRequired;
            }

            if (string.IsNullOrEmpty(model.NewPassword1))
            {
                errorMessage = Resources.ErrorNewPassword1Required;
            }

            if (string.IsNullOrEmpty(model.NewPassword2))
            {
                errorMessage = Resources.ErrorNewPassword2Required;
            }

            if (model.Password == model.NewPassword1)
            {
                errorMessage = Resources.ErrorNewPasswordEqualOldPassword;
            }

            if (model.NewPassword1 != model.NewPassword2)
            {
                errorMessage = Resources.ErrorNewPasswordVerification;
            }

            if (!string.IsNullOrEmpty(errorMessage))
            {               
                model = GetLoginViewModel(model, true, false);
                ModelState.AddModelError("", errorMessage);
                model.Password = "";
                model.NewPassword1 = "";
                model.NewPassword2 = "";
                return View("NewPassword", model);
            }

            return null;
        }   

        /// <summary>
        /// RedirectToApplication
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        private ActionResult RedirectToApplication (LoginViewModel model, string loginAuthenticationValidUrl)
        {
            string queryString = "?" + UadmRequestValues.REQUESTVALUES_PARAMETER + "=" + HttpUtility.UrlEncode(Crypt.EncryptToBase64(model.OsAccount, ViewBag.AuthenticationValues));

            string url = loginAuthenticationValidUrl + queryString;

            UadmLogger.LogInfo("RedirectToApplication: Redirecting user: " + model.Username + " osaccount: " + model.OsAccount);
            UadmLogger.LogInfo("RedirectToApplication: Redirecting url: " + url);

            return Redirect(url);
        }

        /// <summary>
        /// HandleAuthenticatedUser
        /// </summary>
        /// <param name="model"></param>
        /// <param name="user"></param>
        /// <returns></returns>
        private bool HandleAuthenticatedUser(LoginViewModel model, UadmLibUser user)
        {
            UadmAppSettingsApplicationWebParameter uadmAppSettingsApplicationWebParameter = _uadmAppSettings.GetUadmAppSettingsApplicationWebParameter(model.SelectedApplication, model.SelectedEnvironment, model.OriginUrl);

            if (uadmAppSettingsApplicationWebParameter == null)
            {
                model = GetLoginViewModel(model);
                ModelState.AddModelError("", "No ApplicationWebParameter configured for application and environment.");
                return false;
            }

            if (uadmAppSettingsApplicationWebParameter.LoginRedirectUrl == null || uadmAppSettingsApplicationWebParameter.LoginRedirectUrl.Length == 0)
            {
                model = GetLoginViewModel(model);
                ModelState.AddModelError("", "No redirect URL configured for application.");
                return false;
            }

            Dictionary<string, string> claims = CreateTokenClaims(
                model, 
                uadmAppSettingsApplicationWebParameter.UserTokenExpirationMinutes.Value, 
                user,
                uadmAppSettingsApplicationWebParameter.ApplicationId
            );

            string userTokenBase64 = UadmUserToken.CreateUadmUserToken(model.Username, claims);

            // set return values
            ViewBag.LoginAuthenticationValidUrl = uadmAppSettingsApplicationWebParameter.LoginAuthenticationValidUrl;
            ViewBag.RedirectUrl = uadmAppSettingsApplicationWebParameter.LoginRedirectUrl;
            ViewBag.AuthenticationValues = GetRequestValuesAsString (uadmAppSettingsApplicationWebParameter, userTokenBase64);

            _libAppUadmWrapper.UpdateLastLoginDate(claims.GetClaimAsInt(UadmUserToken.CLAIM_USERID), model.Username);

            UadmLogger.LogInfo("HandleAuthenticatedUser: User " + model.Username + " authenticated successfully.");
            UadmLogger.LogInfo("AuthenticationValidUrl: " + ViewBag.LoginAuthenticationValidUrl);

            return true;
        }

        /// <summary>
        /// SetPreviousLoginData
        /// </summary>
        /// <param name="model"></param>
        /// <param name="user"></param>
        /// <returns></returns>
        private bool SetPreviousLoginData(LoginViewModel model, UadmLibUser user)
        {
            // store previous login data if osaccount is provided
            if (!string.IsNullOrEmpty(model.OsAccount))
            {
                UadmLibLoginData loginData = new UadmLibLoginData
                {
                    OsAccount = model.OsAccount ?? UadmUserToken.GetOSAccount(HttpContext),
                    Country = model.SelectedCountry ?? "",
                    Language = model.SelectedLanguage ?? "DE",
                    BranchRegionId = string.IsNullOrEmpty(model.SelectedApplicationItem) ? 0 : int.Parse(model.SelectedApplicationItem),
                    UserId = user.UserId
                };

                return _libAppUadmWrapper.SetPreviousLoginData(loginData, out int errorCode, out string errorMessage);
            }
            return false;
        }

        /// <summary>
        /// GetRequestValuesAsString
        /// </summary>
        /// <param name="uadmAppSettingsApplicationWebParameter"></param>
        /// <param name="userTokenBase64"></param>
        /// <returns></returns>
        private string GetRequestValuesAsString(UadmAppSettingsApplicationWebParameter uadmAppSettingsApplicationWebParameter, string userTokenBase64)
        {
            Dictionary<string, string> requestValues = new Dictionary<string, string>();

            if (!string.IsNullOrEmpty(userTokenBase64))
            {
                requestValues.Add(UadmRequestValues.REQUESTVALUES_PHX_UADM_USERTOKEN, userTokenBase64);
            }

            if (!string.IsNullOrEmpty(uadmAppSettingsApplicationWebParameter.LoginRedirectUrl) && uadmAppSettingsApplicationWebParameter.LoginRedirectUrl.StartsWith("http"))
            {
                if (uadmAppSettingsApplicationWebParameter.LoginRedirectUrl != uadmAppSettingsApplicationWebParameter.LoginAuthenticationValidUrl)
                    requestValues.Add(UadmRequestValues.REQUESTVALUES_PHX_UADM_LOGINREDIRECTURL, uadmAppSettingsApplicationWebParameter.LoginRedirectUrl);
            }

            if (!string.IsNullOrEmpty(uadmAppSettingsApplicationWebParameter.ApplicationRedirectAction))
            {
                requestValues.Add(UadmRequestValues.REQUESTVALUES_PHX_UADM_APPLICATIONACTION, uadmAppSettingsApplicationWebParameter.ApplicationRedirectAction);
            }

            if (!string.IsNullOrEmpty(uadmAppSettingsApplicationWebParameter.ApplicationRedirectController))
            {
                requestValues.Add(UadmRequestValues.REQUESTVALUES_PHX_UADM_APPLICATIONCONTROLLER, uadmAppSettingsApplicationWebParameter.ApplicationRedirectController);
            }

            if (!string.IsNullOrEmpty(uadmAppSettingsApplicationWebParameter.ApplicationRedirectView))
            {
                requestValues.Add(UadmRequestValues.REQUESTVALUES_PHX_UADM_APPLICATIONVIEW, uadmAppSettingsApplicationWebParameter.ApplicationRedirectView);
            }

            return JsonConvert.SerializeObject(requestValues);
        }

        /// <summary>
        /// ChangeLanguage
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        [HttpPost]
        public ActionResult ChangeLanguage(LoginViewModel model)
        {
            UadmLogger.LogInfo($"ChangeLanguage: Changing language to {model.SelectedLanguage}");

            // change culture
            MvcApplication.SetCultureInfoByLanguage(model.SelectedLanguage);

            // reload dropdowns
            GetDropdowns(model);

            // clear model state to avoid validation errors
            ModelState.Clear();

            return View("Index", model);
        }

        /// <summary>
        /// GetEnvironmentsForApplication
        /// </summary>
        /// <param name="applicationKey"></param>
        /// <returns></returns>
        [HttpPost]
        public JsonResult GetEnvironmentsForApplication(string applicationKey)
        {
            Dictionary<string, string> result = GetAppEnvironmentsByUadmAppSettings(applicationKey);
            if (result != null)
                return Json(result.ToArray());

            return Json("");
        }

        /// <summary>
        /// GetApplicationCountries
        /// </summary>
        /// <param name="applicationKey"></param>
        /// <param name="environmentKey"></param>
        /// <returns></returns>
        [HttpPost]
        public JsonResult GetApplicationCountries(string applicationKey, string environmentKey)
        {
            Dictionary<string, string> result = GetApplicationCountriesByUadmLibOrSettings (applicationKey, environmentKey);
            if (result != null)
                 return Json(result.ToArray());

            return Json("");
        }

        /// <summary>
        /// GetApplicationItems
        /// </summary>
        /// <param name="applicationKey"></param>
        /// <param name="environmentKey"></param>
        /// <param name="countryKey"></param>
        /// <returns></returns>
        [HttpPost]
        public JsonResult GetApplicationItems(string applicationKey, string environmentKey, string countryKey)
        {
            Dictionary<string, string> result = GetApplicationItemsByCountryByUadmLib(applicationKey, environmentKey, countryKey);
            if (result != null)
                return Json(result.ToArray());

            return Json("");
        }

        #region Private methods

        /// <summary>
        /// GetLoginViewModel
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        private LoginViewModel GetLoginViewModel(LoginViewModel model, bool loadPreviousLoginData = true, bool setViewLanguage = true)
        {
            if (model == null)
                model = new LoginViewModel();

            try 
            {                 
                return GetLoginViewModel(model.SelectedApplication, model.SelectedEnvironment, model.SelectedCountry, model.SelectedLanguage, model.SelectedApplicationItem, model.OriginUrl, model.OsAccount, loadPreviousLoginData, setViewLanguage);
            }
            catch
            {
                model = new LoginViewModel();
            }
            return model;
        }

        /// <summary>
        /// GetLoginViewModel
        /// </summary>
        /// <param name="applicationKey"></param>
        /// <param name="environmentKey"></param>
        /// <param name="country"></param>
        /// <param name="language"></param>
        /// <param name="branch"></param>
        /// <param name="originUrl"></param>
        /// <returns></returns>
        private LoginViewModel GetLoginViewModel(string applicationKey, string environmentKey, string country, string language, string branch, string originUrl = null, string osAccount = null, bool loadPreviousLoginData = true, bool setViewLanguage = true)
        {
            LoginViewModel model = null;

            bool disableApplications = true;
            bool disableEnvironments = true;

            try
            {
                // If applicationName is not provided, use default application
                if (string.IsNullOrEmpty(applicationKey))
                {
                    disableApplications = false;
                    applicationKey = _uadmAppSettings.GetDefaultApplication();
                }

                if (string.IsNullOrEmpty(environmentKey))
                {
                    disableEnvironments = false;
                }

                if (loadPreviousLoginData == true)
                {
                    model = GetPreviousLoginViewModel(applicationKey, environmentKey, osAccount);
                }

                // If no user data, create new model with defaults
                if (model == null)
                {
                    model = new LoginViewModel
                    {
                        SelectedApplication = applicationKey,
                        SelectedCountry = country,
                        SelectedLanguage = language,
                        SelectedEnvironment = environmentKey,
                        OsAccount = osAccount,
                    };
                }

                model.SelectedApplication = model.SelectedApplication ?? _uadmAppSettings.GetDefaultApplication() ?? _uadmAppSettings.GetSupportedApplicationsAsDictionary().Keys.FirstOrDefault();
                model.SelectedCountry = model.SelectedCountry ?? _uadmAppSettings.GetDefaultCountry() ?? "DE";
                model.SelectedLanguage = model.SelectedLanguage ?? _uadmAppSettings.GetDefaultLanguage(model.SelectedApplication) ?? "DE";
                model.SelectedEnvironment = model.SelectedEnvironment ?? _uadmAppSettings.GetDefaultEnvironment() ?? "DEV";

                // If branch is provided, try to find matching application item
                model.DisableApplications = disableApplications;
                model.DisableEnvironments = disableEnvironments;
                model.OriginUrl = originUrl;
                model.OsAccount = osAccount;

                if (!string.IsNullOrEmpty(originUrl))
                {
                    UadmAppSettingsApplication uadmAppSettingsApplication = _uadmAppSettings.GetApplication(model.SelectedApplication);
                    if (uadmAppSettingsApplication != null)
                    {
                        model.HideCountries = uadmAppSettingsApplication.HideCountries;
                        model.HideLanguages = uadmAppSettingsApplication.HideLanguages;
                        model.HideApplicationItems = uadmAppSettingsApplication.HideApplicationItems;
                        model.HideEnvironments = uadmAppSettingsApplication.HideEnvironments;
                        model.HideApplications = uadmAppSettingsApplication.HideApplications;
                    }

                    // override with ApplicationWebParameter settings
                    UadmAppSettingsApplicationWebParameter uadmAppSettingsApplicationWebParameter = _uadmAppSettings.GetUadmAppSettingsApplicationWebParameter(model.SelectedApplication, model.SelectedEnvironment, model.OriginUrl);
                    if (uadmAppSettingsApplicationWebParameter != null)
                    {
                        if (uadmAppSettingsApplicationWebParameter.HideCountries == true)
                            model.HideCountries = true;

                        if (uadmAppSettingsApplicationWebParameter.HideLanguages == true)
                            model.HideLanguages = true;

                        if (uadmAppSettingsApplicationWebParameter.HideApplicationItems == true)
                            model.HideApplicationItems = true;

                        if (uadmAppSettingsApplicationWebParameter.HideEnvironments == true)
                            model.HideEnvironments = true;

                        if (uadmAppSettingsApplicationWebParameter.HideApplications == true)
                            model.HideApplications = true;
                    }
                }

                GetDropdowns(model);

                model.ShowPasswordChangeButton = WebConfig.GetShowPasswordChangeButton();

                if (setViewLanguage == true)
                {
                    MvcApplication.SetCultureInfoByLanguage(model.SelectedLanguage);
                }
            }
            catch
            {
                UadmLogger.LogError("GetLoginViewModel: Exception try to get data from UadmLib.");
            }
            return model;
        }

        /// <summary>
        /// SetLoginValidationMessages
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        private LoginViewModel SetLoginValidationMessages(LoginViewModel model)
        {
            RemoveLoginValidationMessages(model);

            if (string.IsNullOrEmpty(model.Username))
                ModelState.AddModelError("Username", @Resources.ValidationUsername); 

            if (string.IsNullOrEmpty(model.Password))
                ModelState.AddModelError("Password", @Resources.ValidationPassword); 

            if (string.IsNullOrEmpty(model.SelectedEnvironment) && (model.HideEnvironments == false))
                ModelState.AddModelError("SelectedEnvironment", @Resources.ValidationEnvironment); 
            
            if (string.IsNullOrEmpty(model.SelectedCountry) && (model.HideCountries == false))
                ModelState.AddModelError("SelectedCountry", @Resources.ValidationCountry); 
            
            if (string.IsNullOrEmpty(model.SelectedApplicationItem) && (model.HideApplicationItems == false))
                ModelState.AddModelError("SelectedApplicationItem", @Resources.ValidationBranchRegion);             

            ModelState.AddModelError("", Resources.ErrorFieldsRequired); 

            return model;
        }

        /// <summary>
        /// RemoveLoginValidationMessages
        /// </summary>
        /// <param name="model"></param>
        private void RemoveLoginValidationMessages(LoginViewModel model)
        {
            // Remove existing 
            ModelState.Remove("Username"); 
            ModelState.Remove("Password");
            ModelState.Remove("SelectedEnvironment");
            ModelState.Remove("SelectedCountry");
            ModelState.Remove("SelectedApplicationItem");
            ModelState.Remove("SelectedLanguage");
        }

        /// <summary>
        /// SetLoginViewModelValidationMessages
        /// </summary>
        /// <param name="model"></param>
        /// <returns></returns>
        private LoginViewModel SetNewPasswordValidationMessages(LoginViewModel model)
        {
            RemoveNewPasswordValidationMessages(model);

            if (string.IsNullOrEmpty(model.Username))
                ModelState.AddModelError("Username", @Resources.ValidationUsername); 

            if (string.IsNullOrEmpty(model.Password))
                ModelState.AddModelError("Password", @Resources.ValidationPassword); 

            if (string.IsNullOrEmpty(model.NewPassword1))
                ModelState.AddModelError("NewPassword1", @Resources.ValidationNewPassword1); 

            if (string.IsNullOrEmpty(model.NewPassword2))
                ModelState.AddModelError("NewPassword2", @Resources.ValidationNewPassword2); 

            return model;
        }

        /// <summary>
        /// RemoveNewPasswordValidationMessages
        /// </summary>
        /// <param name="model"></param>
        private void RemoveNewPasswordValidationMessages(LoginViewModel model)
        {
            // Remove existing 
            ModelState.Remove("Username");
            ModelState.Remove("Password");
            ModelState.Remove("NewPassword1");
            ModelState.Remove("NewPassword2");
        }

        /// <summary>
        /// GetPreviousLoginViewModel
        /// </summary>
        /// <param name="applicationKey"></param>
        /// <param name="environmentKey"></param>
        /// <returns></returns>
        private LoginViewModel GetPreviousLoginViewModel(string applicationKey, string environmentKey, string osAccount)
        {
            LoginViewModel model = null;

            if (string.IsNullOrEmpty (applicationKey) || string.IsNullOrEmpty(environmentKey))
            {
                return model;
            }

            // try to get from UadmLib
            try
            {
                UadmLibEnvironment uadmLibEnvironment = GetLibEnvironment(applicationKey, environmentKey);

                if (_libAppUadmWrapper.OpenConnection(uadmLibEnvironment) == false)
                {
                    //model = GetLoginViewModel(model, false, false);
                    ModelState.AddModelError("", "Could not connect to Voodoo service. Please check environment settings.");
                }
                else
                {
                    UadmLibLoginData loginData = _libAppUadmWrapper.GetPreviousLoginData(osAccount);

                    if (loginData != null)
                    {
                        model = new LoginViewModel
                        {
                            SelectedApplication = applicationKey,
                            SelectedEnvironment = environmentKey,
                            Username = loginData.UserName,
                            SelectedApplicationItem = string.IsNullOrEmpty(loginData.BranchRegion) ? null : loginData.BranchRegion,
                            SelectedCountry = string.IsNullOrEmpty(loginData.Country) ? null : loginData.Country,
                            SelectedLanguage = string.IsNullOrEmpty(loginData.Language) ? null : loginData.Language,
                        };
                    }
                }
            }
            catch
            {
                // login failed
                UadmLogger.LogError("GetPreviousLoginViewModel: Exception try to get data from UadmLib.");
            }
            finally
            {
                _libAppUadmWrapper.CloseConnection();
            }

            return model;
        }

        /// <summary>
        /// GetDropdowns
        /// </summary>
        /// <param name="model"></param>
        private void GetDropdowns(LoginViewModel model)
        {
            model.Applications = _uadmAppSettings.GetSupportedApplicationsAsDictionary();
            model.Environments = new Dictionary<string, string>();
            model.Languages = new Dictionary<string, string>();
            model.Countries = new Dictionary<string, string>();
            model.ApplicationItems = new Dictionary<string, string>();

            if (string.IsNullOrEmpty(model.SelectedApplication))
                return;

            if (string.IsNullOrEmpty(model.SelectedEnvironment))
                return;

            // try to get languages from UadmLib
            model.Environments = GetApplicationEnvironmentsByUadmLibOrSettings(model.SelectedApplication);
            model.Languages = GetApplicationLanguagesByUadmLibOrSettings(model.SelectedApplication, model.SelectedEnvironment);
            model.Countries = GetApplicationCountriesByUadmLibOrSettings(model.SelectedApplication, model.SelectedEnvironment);

            // try to get countries from UadmLib
            if (!string.IsNullOrEmpty(model.SelectedCountry))
            {
                model.ApplicationItems = GetApplicationItemsByCountryByUadmLib(model.SelectedApplication, model.SelectedEnvironment, model.SelectedCountry);
            }            
        }

        /// <summary>
        /// GetLibEnvironment
        /// </summary>
        /// <param name="model"></param>
        private UadmLibEnvironment GetLibEnvironment(LoginViewModel model)
        {
           return GetLibEnvironment(model.SelectedApplication, model.SelectedEnvironment);
        }

        /// <summary>
        /// GetLibEnvironment
        /// </summary>
        /// <param name="model"></param>
        private UadmLibEnvironment GetLibEnvironment(string applicationKey, string environmentKey)
        {
            UadmAppSettingsEnvironment appSettingsEnvironment = _uadmAppSettings.GetEnvironment(environmentKey);

            return new UadmLibEnvironment
            {
                Application = applicationKey,
                AppVersion = null,

                Environment = appSettingsEnvironment.Key,
                EnvironmentName = appSettingsEnvironment.Name,

                PrimaryServer = appSettingsEnvironment?.PrimaryServer,
                PrimaryDatabase = appSettingsEnvironment?.PrimaryDatabase,
                SecondaryServer = appSettingsEnvironment?.SecondaryServer,
                SecondaryDatabase = appSettingsEnvironment?.SecondaryDatabase,
                VoodooPipeName = appSettingsEnvironment?.VoodooPipeName,
                VoodooPipePort = appSettingsEnvironment.VoodooPipePort,
                VoodooPipePortText = appSettingsEnvironment.VoodooPipePort.ToString()
            };
        }

        /// <summary>
        /// GetAppEnvironmentsByUadmLib
        /// </summary>
        /// <returns></returns>
        private Dictionary<string, string> GetAppEnvironmentsByUadmAppSettings(string applicationKey)
        {
            Dictionary<string, string> allEnvironments = _uadmAppSettings.GetSupportedEnvironmentsAsDictionary();

            Dictionary<string, Dictionary<string,UadmAppSettingsApplicationWebParameter>> appEnvironments = _uadmAppSettings.GetApplication(applicationKey).SupportedEnvironments;

            Dictionary<string, string> returnDict = new Dictionary<string, string>();

            if ((allEnvironments != null) && (appEnvironments != null))
            {
                foreach (string env in appEnvironments.Keys)
                {
                    if (allEnvironments.ContainsKey(env))
                    {
                        returnDict[env] = allEnvironments[env];
                    }
                }
            }

            return returnDict;
        }

        /// <summary>
        /// IsExcludedOSAccount
        /// </summary>
        /// <param name="osAccount"></param>
        /// <returns></returns>
        private bool IsExcludedOSAccount (string osAccount)
        {
            osAccount = osAccount ?? "";
            osAccount = osAccount.Trim();

            // check for empty os account and if allowed
            if (string.IsNullOrEmpty(osAccount))
                return WebConfig.GetUadmAllowEmptyOsAccount(false) == false;

            string[] excludedOsAccounts = WebConfig.GetUadmExcludedOsAccounts("").Split(';');

            if ((excludedOsAccounts != null) && (excludedOsAccounts.Length > 0))    
            {
                foreach (string excludedOsAccount in excludedOsAccounts)
                {
                    string value = excludedOsAccount ?? "";
                    value = value.Trim();

                    if (string.Compare(osAccount, value, StringComparison.InvariantCultureIgnoreCase) == 0)
                        return true;
                }
            }
            return false;
        }

        #endregion

        #region UADMLIB methods 

        /// <summary>
        /// GetErrorMessageFromResponse
        /// </summary>
        /// <param name="errorCode"></param>
        /// <param name="errorMessage"></param>
        /// <returns></returns>
        private string GetErrorMessageFromResponse(int errorCode, string errorMessage, bool useNewPasswordRules = true)
        {
            switch ((UADM_JSON_RESPONSE_ERRORCODES) errorCode)
            {
                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_OK:
                    return "";  
                
                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_ERROR:
                    return Resources.ErrorUadmConnection;

                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_MANAGER_IS_NULL:
                    return Resources.ErrorResponseManagerIsNull;
                
                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_LOGININTERFACE_IS_NULL:
                    return Resources.ErrorResponseLoginInterfaceIsNull;

                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_COULD_NOT_INIITIALIZE:
                    return Resources.ErrorResponseCouldNotInitialize;

                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_NO_ENVIRONMENT_FOUND:
                    return Resources.ErrorResponseNoEnvironmentFound;

                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_COULD_NOT_VERIFY_USER:
                    return Resources.ErrorResponseCouldNotVerifyUser;

                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_USER_IS_NOT_AUTHENTICATED:
                    return Resources.ErrorResponseUserIsNotAuthenticated;

                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_USER_IS_NOT_ACTIVATED:
                    return Resources.ErrorResponseUserIsNotActivated;

                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_LAST_PASSWORD_VERIFY_FAILED:
                    return Resources.ErrorResponseLastPasswordVerifyFailed;

                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_HISTORY_VERIFY_FAILED:
                    return Resources.ErrorResponseHistoryVerifyFailed;

                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_NO_PREVIOUS_LOGIN_DATA_FOUND:
                    return Resources.ErrorResponseNoPreviousLoginDataFound;

                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_PASSWORD_NOT_VALID:

                    string message = "";

                    if (useNewPasswordRules)
                        message = Resources.ErrorResponsePasswordNotValidNewRules;
                    else
                        message = Resources.ErrorResponsePasswordNotValidOldRules;

                    if (WebConfig.GetUseDetailedMessage())
                        message += " (" + errorMessage + ")";

                    return message;

                case UADM_JSON_RESPONSE_ERRORCODES.UADM_JSON_RESPONSE_SAVE_PASSWORD_FAILED:
                    return Resources.ErrorResponseSavePasswordFailed;

                default:
                    break;
            }

            return errorMessage;
        }

        /// <summary>
        /// GetApplicationEnvironmentsByUadmLibOrSettings
        /// </summary>
        /// <returns></returns>
        private Dictionary<string, string> GetApplicationEnvironmentsByUadmLibOrSettings(string applicationKey)
        {           
            return _uadmAppSettings.GetApplicationEnvironmentsAsDictionary(applicationKey); ;
        }

        /// <summary>
        /// GetApplicationCountriesByUadmLibOrSettings
        /// </summary>
        /// <returns></returns>
        private Dictionary<string, string> GetApplicationCountriesByUadmLibOrSettings(string applicationKey, string environmentKey)
        {
            Dictionary<string, string> returnDict = new Dictionary<string, string>();

            try
            {
                UadmLibEnvironment uadmLibEnvironment = GetLibEnvironment(applicationKey, environmentKey);

                if (_libAppUadmWrapper.OpenConnection(uadmLibEnvironment) != false)
                {
                    Dictionary<string, UadmLibCountry> countries = _libAppUadmWrapper.GetAppCountries();
                    if (countries != null)
                    {
                        returnDict = countries.ToDictionary(
                            kvp => kvp.Key,
                            kvp => kvp.Value.Name
                        );
                    }
                    else
                        UadmLogger.LogError("GetApplicationCountriesByUadmLibOrSettings: Countries not found.");
                }
                else
                    UadmLogger.LogError("GetApplicationCountriesByUadmLibOrSettings: OpenConnection fails.");
            }
            catch (Exception ex)
            {
                UadmLogger.LogError("GetApplicationCountriesByUadmLibOrSettings: " + ex.Message);
                UadmLogger.LogError("Trace: " + ex.StackTrace);
            }
            finally
            {
                _libAppUadmWrapper.CloseConnection();
            }

            return returnDict;
        }

        /// <summary>
        /// GetApplicationLanguagesByUadmLibOrSettings
        /// </summary>
        /// <returns></returns>
        private Dictionary<string, string> GetApplicationLanguagesByUadmLibOrSettings(string applicationKey, string environmentKey)
        {
            if (_useAppSettings == true)
                return _uadmAppSettings.GetSupportedLanguagesAsDictionary(applicationKey); 

            Dictionary<string, string> returnDict = new Dictionary<string, string>();

            try
            {
                UadmLibEnvironment uadmLibEnvironment = GetLibEnvironment(applicationKey, environmentKey);

                if (_libAppUadmWrapper.OpenConnection(uadmLibEnvironment) != false)
                {
                    Dictionary<string, UadmLibLanguage> languages = _libAppUadmWrapper.GetAppLanguages();
                    if (languages == null)
                        return _uadmAppSettings.GetSupportedLanguagesAsDictionary(applicationKey);
                    
                    return languages.ToDictionary(
                        kvp => kvp.Key,
                        kvp => kvp.Value.Name
                    );
                    
                }
            }
            catch
            {
            }
            finally
            {
                _libAppUadmWrapper.CloseConnection();
            }

            return returnDict;           
        }

        /// <summary>
        /// GetApplicationItemsByCountryByUadmLib
        /// </summary>
        /// <returns></returns>
        private Dictionary<string, string> GetApplicationItemsByCountryByUadmLib(string applicationKey, string environmentKey, string countryKey)
        {
            // TODO: only for TEST
            //if (_useAppSettings == true)
            //    return null;

            Dictionary<string, string> returnDict = new Dictionary<string, string>();

            try
            {
                UadmLibEnvironment uadmLibEnvironment = GetLibEnvironment(applicationKey, environmentKey);


                if (_libAppUadmWrapper.OpenConnection(uadmLibEnvironment) != false)
                {
                    Dictionary<string, UadmLibApplicationItem> items = _libAppUadmWrapper.GetAppItemsByCountry(countryKey);
                    if (items != null)
                    {
                        returnDict = items.ToDictionary(
                            kvp => kvp.Key,
                            kvp => kvp.Value.RegionBranchName
                        );
                    }
                }
            }
            catch
            {
            }
            finally
            {
                _libAppUadmWrapper.CloseConnection();
            }

            return returnDict;
        }

        /// <summary>
        /// CreateTokenClaims
        /// </summary>
        /// <param name="model"></param>
        /// <param name="expires"></param>
        /// <param name="user"></param>
        /// <returns></returns>
        private Dictionary<string, string> CreateTokenClaims(LoginViewModel model, int expires, UadmLibUser user, string applicationId = "")
        {
            // create tokendata
            Dictionary<string, string> claims = new Dictionary<string, string>
            {
                { UadmUserToken.CLAIM_CREATED, DateTime.Now.Ticks.ToString() },
                { UadmUserToken.CLAIM_MODIFIED, DateTime.Now.Ticks.ToString() },
                { UadmUserToken.CLAIM_EXPIRES, expires.ToString() },
                { UadmUserToken.CLAIM_USERNAME, model.Username },
                { UadmUserToken.CLAIM_USERID, user?.UserId.ToString() },
                { UadmUserToken.CLAIM_EMAIL, user?.Email },
                { UadmUserToken.CLAIM_FIRSTNAME, user?.Firstname },
                { UadmUserToken.CLAIM_LASTNAME, user?.Lastname },
                { UadmUserToken.CLAIM_DOMAINACCOUNT, user?.DomainAccount },
                { UadmUserToken.CLAIM_DESCRIPTION, user?.Description },
                { UadmUserToken.CLAIM_OSACCOUNT, string.IsNullOrEmpty (user?.OSAccount) ? (string.IsNullOrEmpty (model.OsAccount) ? "" : model.OsAccount)  : user.OSAccount },
                { UadmUserToken.CLAIM_ISAUTHENTICATED, user.IsAuthenticated.ToString() },
                { UadmUserToken.CLAIM_ISACTIVE, user.IsActive.ToString()  },
                { UadmUserToken.CLAIM_ISPASSWORDCHANGEREQUIRED, user.IsPasswordChangeRequired.ToString()  },
                { UadmUserToken.CLAIM_LASTPASSWORDCHANGEDATE, user?.LastPasswordChangeDate.ToString() },
                { UadmUserToken.CLAIM_LASTLOGINDATE, user?.LastLoginDate.ToString() },
                { UadmUserToken.CLAIM_APPLICATION, model.SelectedApplication },
                { UadmUserToken.CLAIM_APPLICATIONID, applicationId },
                { UadmUserToken.CLAIM_ENVIRONMENT, model.SelectedEnvironment },
                { UadmUserToken.CLAIM_COUNTRY, model.SelectedCountry },
                { UadmUserToken.CLAIM_LANGUAGE, model.SelectedLanguage },
                { UadmUserToken.CLAIM_BRANCH, model.SelectedApplicationItem },
            };
            return claims;
        }

        #endregion
    }
}
