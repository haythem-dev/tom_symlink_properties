using Newtonsoft.Json;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using UadmCommon.Attributes;

namespace UadmWebLoginService.Models
{
    public class LoginViewModel
    {
        [Required]
        public string Username { get; set; }

        [Required]
        [JsonIgnore]
        public string Password { get; set; }

        [Required]
        [JsonIgnore]
        public string NewPassword1 { get; set; } = "AAAAAA";

        [Required]
        [JsonIgnore]
        public string NewPassword2 { get; set; } = "BBBBBB";

        [JsonIgnore]
        public bool DisableApplications { get; set; } = false;

        [JsonIgnore]
        public bool DisableEnvironments { get; set; } = false;

        [JsonIgnore]
        public bool HideCountries { get; set; } = false;
        
        [JsonIgnore]
        public bool HideLanguages { get; set; } = false;

        [JsonIgnore]
        public bool HideApplicationItems { get; set; } = false;

        [JsonIgnore]
        public bool HideEnvironments { get; set; } = false;

        [JsonIgnore]
        public bool HideApplications { get; set; } = false;

        [JsonIgnore]
        public bool IsChangePasswordRequired { get; set; } = true;

        [JsonIgnore]
        public string OriginUrl { get; set; }

        [JsonIgnore]
        public string OsAccount { get; set; }

        [JsonIgnore]
        public bool ShowPasswordChangeButton { get; set; } = false;

        // Add these for ComboBox
        [Required]
        public string SelectedApplication { get; set; }

        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public Dictionary<string, string> Applications { get; set; } = new Dictionary<string, string>();

        [DynamicRequiredOnHide("HideCountries")]
        public string SelectedCountry { get; set; }

        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public Dictionary<string, string> Countries { get; set; } = new Dictionary<string, string>();

        [DynamicRequiredOnHide("HideLanguages")]
        public string SelectedLanguage{ get; set; }

        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public Dictionary<string, string> Languages { get; set; } = new Dictionary<string, string>();

        [Required]
        public string SelectedEnvironment { get; set; }

        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public Dictionary<string, string> Environments { get; set; } = new Dictionary<string, string> ();

        [DynamicRequiredOnHide("HideApplicationItems")]
        public string SelectedApplicationItem { get; set; }

        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public Dictionary<string, string> ApplicationItems { get; set; } = new Dictionary<string, string>();
    }
}