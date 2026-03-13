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
    [RoutePrefix("api/parameters")]
    [UadmRequestAuthorize]
    [AllowAnonymous]
    public class ParametersController : BaseController
    {
        private LibAppUadmService _libAppUadmService = new LibAppUadmService(new HttpContextWrapper(System.Web.HttpContext.Current));

        /// <summary>
        /// GetUadmAllAppParameters
        /// </summary>
        /// <returns></returns>
        [HttpGet]
        [Route("all")]
        public async Task<ActionResult> GetUadmAllAppParameters()
        {
            UadmLogger.LogInfo("GetUadmAllAppParameters called.");

            List<UadmLibParameter> result = await _libAppUadmService.GetUadmAllAppParametersAsync(HttpContext);
            if (result == null)
                return Content("{}", "application/json");

            string json = JsonConvert.SerializeObject(result);

            return Content(json, "application/json");
        }

        /// <summary>
        /// GetUadmAppParametersInfo
        /// </summary>
        /// <param name="para1"></param>
        /// <param name="para2"></param>
        /// <param name="parameterName"></param>
        /// <returns></returns>
        [HttpGet]
        [Route("parameterinfo")]
        // GET: App Permission  
        public async Task<ActionResult> GetUadmAppParametersInfo(string para1, string para2, string parameterName)
        {
            UadmLogger.LogInfo("GetUadmAppParametersInfo called.");

            List<UadmLibParameter> result = await _libAppUadmService.GetUadmAppParametersInfoAsync(HttpContext, para1, para2, parameterName);
            if (result == null)
                return Content("{}", "application/json");

            string json = JsonConvert.SerializeObject(result);

            return Content(json, "application/json");                
        }
    }
}
