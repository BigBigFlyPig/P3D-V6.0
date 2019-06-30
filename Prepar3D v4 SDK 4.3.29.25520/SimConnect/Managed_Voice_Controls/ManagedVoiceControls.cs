using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Speech;
using System.Speech.Synthesis;
using System.Speech.Recognition;
using System.Windows.Forms;

using LockheedMartin.Prepar3D.SimConnect;
using System.Runtime.InteropServices;

namespace Managed_Voice_Controls
{
    class ManagedVoiceControls
    {
        enum EVENTS
        {
            PAUSE = 0,
            UNPAUSE,
        };

        Dictionary<string, EVENTS> phrases = new Dictionary<string, EVENTS>()
        {
            { "pause prepared", EVENTS.PAUSE },
            { "unpause prepared", EVENTS.UNPAUSE }
        };

        enum GROUPID
        {
            FLAG = 2000000000,
        };

        SimConnect simconnect = null;
        GrammarBuilder grammarBuilder = null;
        SpeechRecognitionEngine recognitionEngine = null;
        bool bVoiceRecognitionEnabled = true;
        bool done = false;
        const int WM_USER_SIMCONNECT = 0x0402;

        public IntPtr Handle { get; private set; }

        public ManagedVoiceControls()
        {
        }

        public void Run()
        {
            Connect();
            BuildGrammar();
            Listen();
        }

        public void Connect()
        {
            try
            {
                simconnect = new SimConnect("Managed Voice Control", this.Handle, WM_USER_SIMCONNECT, null, 0);
                simconnect.MapClientEventToSimEvent(EVENTS.PAUSE, "PAUSE_ON");
                simconnect.MapClientEventToSimEvent(EVENTS.UNPAUSE, "PAUSE_OFF");
            }
            catch (Exception ex)
            {
                Console.WriteLine("Sim Connect unable to connect to Prepar3D!\n\n{0}\n\n{1}", ex.Message, ex.StackTrace);
            }
        }

        public void BuildGrammar()
        {
            grammarBuilder = new GrammarBuilder();
            recognitionEngine = new SpeechRecognitionEngine();
            Choices phraseChoices = new Choices();

            foreach (KeyValuePair<string, EVENTS> i in phrases)
            {
                phraseChoices.Add(i.Key);
            }

            grammarBuilder.Append(phraseChoices);
            Grammar grammar = new Grammar(grammarBuilder);

            recognitionEngine.SetInputToDefaultAudioDevice();
            recognitionEngine.LoadGrammar(grammar);
        }

        public void Listen()
        {
            recognitionEngine.SpeechRecognized += new EventHandler<SpeechRecognizedEventArgs>(OnSpeechRecognized);
            recognitionEngine.RecognizeAsync(RecognizeMode.Multiple);

            while (!done)
            {
                string key = Console.ReadKey(true).Key.ToString();

                if (key.ToUpper() == "T")
                {
                    ToggleVoiceRecognition();
                }
            }
        }

        public void ToggleVoiceRecognition()
        {
            if (bVoiceRecognitionEnabled)
            {
                recognitionEngine.RecognizeAsyncStop();
                bVoiceRecognitionEnabled = false;
                Console.WriteLine("Voice recognition disabled\n");
            }
            else
            {
                recognitionEngine.RecognizeAsync(RecognizeMode.Multiple);
                bVoiceRecognitionEnabled = true;
                Console.WriteLine("Voice recognition enabled\n");
            }
        }

        public void OnSpeechRecognized(object sender, SpeechRecognizedEventArgs e)
        {
            if (e.Result.Confidence < .90 || !bVoiceRecognitionEnabled)
            {
                return;
            }

            switch (phrases[e.Result.Text])
            {
                case EVENTS.PAUSE:
                    simconnect.TransmitClientEvent((uint)SimConnect.SIMCONNECT_OBJECT_ID_USER, EVENTS.PAUSE, (uint)0, GROUPID.FLAG, SIMCONNECT_EVENT_FLAG.GROUPID_IS_PRIORITY);
                    Console.WriteLine("Pausing Prepar3D\n");
                    break;

                case EVENTS.UNPAUSE:
                    simconnect.TransmitClientEvent((uint)SimConnect.SIMCONNECT_OBJECT_ID_USER, EVENTS.UNPAUSE, (uint)0, GROUPID.FLAG, SIMCONNECT_EVENT_FLAG.GROUPID_IS_PRIORITY);
                    Console.WriteLine("Unpausing Prepar3D\n");
                    break;

                default:
                    break;
            }
        }

        public void clearGrammar()
        {
            recognitionEngine.UnloadAllGrammars();
        }
    }
}
