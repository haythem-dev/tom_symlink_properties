using Newtonsoft.Json;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Web;
using System.Web.Mvc;
using UadmCommon.Classes;
using UadmCommon.Models;
using UadmWebLoginService.Classes;

namespace UadmWebLoginService.Controllers
{
    [RoutePrefix("api/permissions")]
    [UadmRequestAuthorize]
    [AllowAnonymous]
    public class PermissionsController : BaseController
    {
        private LibAppUadmService _libAppUadmService = new LibAppUadmService(new HttpContextWrapper(System.Web.HttpContext.Current));

        /// <summary>
        /// GetUadmAllUserPermissions
        /// </summary>
        /// <returns></returns>
        [HttpGet]
        [Route("all")]
        public async Task<ActionResult> GetUadmAllUserPermissions()
        {
            UadmLogger.LogInfo("GetUadmAllUserPermissions called.");    

            List<UadmLibPermission> result = await _libAppUadmService.GetUadmAllUserPermissionsAsync(HttpContext, false);
            if (result == null)
                return Content("{}", "application/json");

            string json = JsonConvert.SerializeObject(result);

            UadmLogger.LogInfo("GetUadmAllUserPermissions success.");

            return Content(json, "application/json");
        }

        /// <summary>
        /// GetUadmAllUserPermissionsByBranch
        /// </summary>
        /// <returns></returns>
        [HttpGet]
        [Route("branch")]
        public async Task<ActionResult> GetUadmAllUserPermissionsByBranch()
        {
            UadmLogger.LogInfo("GetUadmAllUserPermissions called.");

            List<UadmLibPermission> result = await _libAppUadmService.GetUadmAllUserPermissionsAsync(HttpContext, true);
            if (result == null)
                return Content("{}", "application/json");

            string json = JsonConvert.SerializeObject(result);

            UadmLogger.LogInfo("GetUadmAllUserPermissions success.");

            return Content(json, "application/json");
        }

        /// <summary>
        /// GetUadmUserPermissionInfo
        /// </summary>
        /// <param name="permissionRight"></param>
        /// <returns></returns>
        [HttpGet]
        [Route("permissioninfo")]
        public async Task<ActionResult> GetUadmUserPermissionInfo(string permissionRight)
        {
            UadmLogger.LogInfo("GetUadmUserPermissionInfo called.");

            UadmLibPermission result = await _libAppUadmService.GetUadmUserPermissionInfoAsync(HttpContext, permissionRight);
            if (result == null)
                return Content("{}", "application/json");

            string json = JsonConvert.SerializeObject(result);

            return Content(json, "application/json");
        }

        /// <summary>
        /// VerifyUserPermission
        /// </summary>
        /// <param name="permissionRight"></param>
        /// <returns></returns>
        [HttpGet]
        [Route("verifypermission")]     
        public async Task<ActionResult> VerifyUserPermission(string permissionRight)
        {
            UadmLogger.LogInfo("VerifyUserPermission called.");

            bool result = await _libAppUadmService.VerifyUserPermissionAsync(HttpContext, permissionRight);

            string json = JsonConvert.SerializeObject(result);

            return Content(json, "application/json");
        }
    }
}
