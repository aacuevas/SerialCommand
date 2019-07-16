#include "SerialCommandEditor.h"
#include "SerialCommandNode.h"

using namespace SerialCommand;

SerialCommandEditor::SerialCommandEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors)
	: GenericEditor(parentNode, useDefaultParameterEditors)
{
	desiredWidth = 250;

	m_comLabel = new Label("COM Label", "Port:");
	m_comLabel->setBounds(10, 25, 50, 20);
	addAndMakeVisible(m_comLabel);

	m_comSelector = new ComboBox("COM Selector");
	m_comSelector->setBounds(60, 25, 180, 20);
	m_comSelector->addListener(this);
	addAndMakeVisible(m_comSelector);

	m_baudLabel = new Label("Rate label", "Rate:");
	m_baudLabel->setBounds(10, 50, 50, 20);
	addAndMakeVisible(m_baudLabel);

	m_baudSelector = new ComboBox("Rate Selector");
	m_baudSelector->setBounds(60, 50, 180, 20);
	m_baudSelector->addListener(this);
	addAndMakeVisible(m_baudSelector);

	m_triggerLabel = new Label("Trigger Label", "Trigger:");
	m_triggerLabel->setBounds(10, 75, 50, 20);
	addAndMakeVisible(m_triggerLabel);

	m_triggerSelector = new ComboBox("Trigger selector");
	m_triggerSelector->setBounds(60, 75, 180, 20);
	m_triggerSelector->addListener(this);
	addAndMakeVisible(m_triggerSelector);

	m_commandLabel = new Label("Command label", "CMD:");
	m_commandLabel->setBounds(10, 102, 50, 20);
	addAndMakeVisible(m_commandLabel);

	m_commandLine = new TextEditor("Command");
	m_commandLine->setBounds(60, 100, 180, 25);
	addAndMakeVisible(m_commandLine);

	SerialCommandNode* parent = static_cast<SerialCommandNode*>(getProcessor());
	m_comSelector->addItemList(parent->getComList(), 1);

	Array<int> rates = parent->getBaudRates();
	for (int i = 0; i < rates.size(); i++)
	{
		m_baudSelector->addItem(String(rates[i]), i + 1);
	}
	
}

SerialCommandEditor::~SerialCommandEditor()
{

}

void SerialCommandEditor::updateSettings()
{
	m_eventList.clear();
	m_triggerSelector->clear();
	m_triggerSelector->addItem("None", 1);
	GenericProcessor* proc = getProcessor();
	int nextItem = 2;
	int selectedId = 1;

	int nEvents = proc->getTotalEventChannels();

	for (int i = 0; i < nEvents; i++)
	{
		const EventChannel* event = proc->getEventChannel(i);
		if (event->getChannelType() == EventChannel::TTL)
		{
			SelectedEvent selected;
			bool eventMatch = false;
			if (event->getSourceNodeID() == m_oldEventSourceId &&
				event->getSubProcessorIdx() == m_oldEventStreamIdx &&
				event->getSourceIndex() == m_oldEventIndex)
				eventMatch = true;

			selected.eventIndex = i;
			int nChans = event->getNumChannels();
			for (int c = 0; c < nChans; c++)
			{
				selected.channelIndex = c;
				String name = event->getSourceName() + " (TTL" + String(c + 1) + ")";
				m_eventList.add(selected);
				m_triggerSelector->addItem(name, nextItem);

				if (eventMatch && c == m_oldEventChannel)
					selectedId = nextItem;

				nextItem = nextItem + 1;
			}
		}
	}
	m_triggerSelector->setSelectedId(selectedId, sendNotification);
}

void SerialCommandEditor::comboBoxChanged(ComboBox* combo)
{
	SerialCommandNode* proc = static_cast<SerialCommandNode*>(getProcessor());
	if (combo == m_comSelector)
	{
		proc->setComPort(combo->getText());
	}
	else if (combo == m_baudSelector)
	{
		proc->setComRateIndex(combo->getSelectedId() - 1);
	}
	else if (combo == m_triggerSelector)
	{
		if (combo->getSelectedId() > 1)
		{
			int index = combo->getSelectedId() - 2;
			//invalidate the input first to ensure there are no race conditions
			proc->setParameter(0, -1);
			proc->setParameter(1, m_eventList[index].channelIndex);
			proc->setParameter(0, m_eventList[index].eventIndex);

			const EventChannel* chan = proc->getEventChannel(m_eventList[index].eventIndex);
			m_oldEventSourceId = chan->getSourceNodeID();
			m_oldEventStreamIdx = chan->getSubProcessorIdx();
			m_oldEventIndex = chan->getSourceIndex();
			m_oldEventChannel = m_eventList[index].channelIndex;
		}
		else
		{
			proc->setParameter(0, -1);
			m_oldEventSourceId = 0;
		}
	}
}

void SerialCommandEditor::startAcquisition()
{
	static_cast<SerialCommandNode*>(getProcessor())->setCommand(m_commandLine->getText());
	m_commandLine->setEnabled(false);
	m_comSelector->setEnabled(false);
	m_baudSelector->setEnabled(false);
}

void SerialCommandEditor::stopAcquisition()
{
	m_commandLine->setEnabled(true);
	m_comSelector->setEnabled(true);
	m_baudSelector->setEnabled(true);
}

