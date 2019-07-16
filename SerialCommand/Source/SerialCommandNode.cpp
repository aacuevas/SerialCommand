#include "SerialCommandNode.h"
#include "SerialCommandEditor.h"
#include <vector>

using namespace SerialCommand;

//Change al names for the relevant ones, including "Processor Name"
SerialCommandNode::SerialCommandNode() : GenericProcessor("Serial Command")
{
	m_selectedBaud = -1;
	m_selectedEvent.set(-1);
	m_selectedChannel.set(-1);
	setProcessorType(PluginProcessorType::PROCESSOR_TYPE_SINK);
}

SerialCommandNode::~SerialCommandNode()
{
	serial.close();
}

AudioProcessorEditor* SerialCommandNode::createEditor()
{
	editor = new SerialCommandEditor(this, true);
	return editor;
}

void SerialCommandNode::process(AudioSampleBuffer& buffer)
{
	/** 
	If the processor needs to handle events, this method must be called onyle once per process call
	If spike processing is also needing, set the argument to true
	*/
	checkForEvents(false);	 
	serial.flush(true, false);
}

bool SerialCommandNode::isReady()
{
	if (m_selectedBaud < 0 || m_selectedPort.isEmpty())
	{
		CoreServices::sendStatusMessage("SerialCommand: Port configuration not valid");
		return false;
	}
	std::cout << " Attempting to connect to serial " << m_selectedPort.toStdString() << "@" << c_baudrates[m_selectedBaud] << std::endl;
	if (serial.setup(m_selectedPort.toStdString(), c_baudrates[m_selectedBaud]))
	{
		return true;
	}
	else
	{
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "SerialInput connection error!", "Could not connect to specified serial device. Check log files for details.");
		return false;
	}
}

bool SerialCommandNode::disable()
{
	serial.close();
	return true;
}

void SerialCommandNode::setParameter(int index, float parameter)
{
	if (index == 0)
	{
		m_selectedEvent.set(static_cast<int>(parameter));
	}
	else if (index == 1)
	{
		m_selectedChannel.set(static_cast<int>(parameter));
	}
}

void SerialCommandNode::setComPort(String comPort)
{
#ifdef WIN32
	comPort = "\\\\.\\" + comPort;
#endif
	m_selectedPort = comPort;
}

void SerialCommandNode::setComRateIndex(int idx)
{
	m_selectedBaud = idx;
}

Array<int> SerialCommandNode::getBaudRates() const
{
	Array<int> allBaudrates(c_baudrates, 12);
	return allBaudrates;
}

StringArray SerialCommandNode::getComList()
{
	vector<ofSerialDeviceInfo> allDeviceInfos = serial.getDeviceList();

	StringArray allDevices;

	for (int i = 0; i < allDeviceInfos.size(); ++i)
	{
		allDevices.add(allDeviceInfos[i].getDeviceName());
	}

	return allDevices;
}

void SerialCommandNode::setCommand(String cmd)
{
	if (cmd.endsWith("\\n") & !cmd.endsWith("\\\\n"))
	{
		m_command = cmd.dropLastCharacters(2) + "\n";
	}
	else
		m_command = cmd;
}

void SerialCommandNode::handleEvent(const EventChannel* eventInfo, const MidiMessage& event, int samplePosition)
{
	int eventIndex = m_selectedEvent.get();
	if (eventIndex < 0) return;
	int channelIndex = m_selectedChannel.get();

	if (eventInfo->getChannelType() == EventChannel::TTL && eventInfo == eventChannelArray[eventIndex])
	{
		TTLEventPtr ttl = TTLEvent::deserializeFromMessage(event, eventInfo);
		if (ttl->getChannel() == channelIndex && ttl->getState())
		{
			serial.writeBytes(reinterpret_cast<unsigned char*>(m_command.toUTF8().getAddress()), m_command.getNumBytesAsUTF8());
		}
	}

}

const int SerialCommandNode::c_baudrates[12] =
{
	300
	, 1200
	, 2400
	, 4800
	, 9600
	, 14400
	, 19200
	, 28800
	, 38400
	, 57600
	, 115200
	, 230400
};