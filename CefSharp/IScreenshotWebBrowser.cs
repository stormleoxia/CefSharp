using System.Drawing;
using System.Threading.Tasks;

namespace CefSharp
{
    /// <summary>
    /// Provides screenshots of the current displayed content in the web browser 
    /// </summary>
    public interface IScreenshotWebBrowser
    {
        Task<Bitmap> ScreenshotAsync(bool ignoreExistingScreenshot = false);
    }
}
