//This prevents include loops. We recommend changing the macro to a name suitable for your plugin
#ifndef PROCESSORPLUGIN_H_DEFINED
#define PROCESSORPLUGIN_H_DEFINED

#include <ProcessorHeaders.h>
#include <SerialLib.h>

//namespace must be an unique name fot your plugin
namespace SerialCommand
{
	class SerialCommandNode : public GenericProcessor
	{
	public:
		/** The class constructor, used to initialize any members. */
		SerialCommandNode();

		/** The class destructor, used to deallocate memory */
		~SerialCommandNode();

		/** Indicates if the processor has a custom editor. Defaults to false */
		bool hasEditor() const override { return true; }

		bool isSink() const override { return true; }

		/** If the processor has a custom editor, this method must be defined to instantiate it. */
		AudioProcessorEditor* createEditor() override;

		/** Defines the functionality of the processor.

		The process method is called every time a new data buffer is available.

		Processors can either use this method to add new data, manipulate existing
		data, or send data to an external target (such as a display or other hardware).

		Continuous signals arrive in the "buffer" variable, event data (such as TTLs
		and spikes) is contained in the "events" variable.
		*/
		void process(AudioSampleBuffer& buffer) override;

		/** Handles events received by the processor

		Called automatically for each received event whenever checkForEvents() is called from process()		
		*/
		void handleEvent(const EventChannel* eventInfo, const MidiMessage& event, int samplePosition) override;

		/** Optional method called every time the signal chain is refreshed or changed in any way.

		Allows the processor to handle variations in the channel configuration or any other parameter
		passed down the signal chain. The processor can also modify here the settings structure, which contains
		information regarding the input and output channels as well as other signal related parameters. Said
		structure shouldn't be manipulated outside of this method.

		*/
		//void updateSettings() override;

		bool isReady() override;

		bool disable() override;

		//0-event index 1-channel index
		void setParameter(int index, float parameter) override;

		StringArray getComList();

		Array<int> getBaudRates() const;

		void setComPort(String comPort);

		void setComRateIndex(int idx);

		void setCommand(String cmd);

	private:
		String m_command;
		String m_selectedPort;
		int m_selectedBaud;
		Atomic<int> m_selectedEvent;
		Atomic<int> m_selectedChannel;

		ofSerial serial;

		static const int c_baudrates[12];

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SerialCommandNode);

	};
}

#endif