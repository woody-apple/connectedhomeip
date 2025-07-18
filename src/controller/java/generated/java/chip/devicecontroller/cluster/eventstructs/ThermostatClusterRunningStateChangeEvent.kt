/*
 *
 *    Copyright (c) 2023 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
package chip.devicecontroller.cluster.eventstructs

import chip.devicecontroller.cluster.*
import java.util.Optional
import matter.tlv.ContextSpecificTag
import matter.tlv.Tag
import matter.tlv.TlvReader
import matter.tlv.TlvWriter

class ThermostatClusterRunningStateChangeEvent(
  val previousRunningState: Optional<UInt>,
  val currentRunningState: UInt,
) {
  override fun toString(): String = buildString {
    append("ThermostatClusterRunningStateChangeEvent {\n")
    append("\tpreviousRunningState : $previousRunningState\n")
    append("\tcurrentRunningState : $currentRunningState\n")
    append("}\n")
  }

  fun toTlv(tlvTag: Tag, tlvWriter: TlvWriter) {
    tlvWriter.apply {
      startStructure(tlvTag)
      if (previousRunningState.isPresent) {
        val optpreviousRunningState = previousRunningState.get()
        put(ContextSpecificTag(TAG_PREVIOUS_RUNNING_STATE), optpreviousRunningState)
      }
      put(ContextSpecificTag(TAG_CURRENT_RUNNING_STATE), currentRunningState)
      endStructure()
    }
  }

  companion object {
    private const val TAG_PREVIOUS_RUNNING_STATE = 0
    private const val TAG_CURRENT_RUNNING_STATE = 1

    fun fromTlv(tlvTag: Tag, tlvReader: TlvReader): ThermostatClusterRunningStateChangeEvent {
      tlvReader.enterStructure(tlvTag)
      val previousRunningState =
        if (tlvReader.isNextTag(ContextSpecificTag(TAG_PREVIOUS_RUNNING_STATE))) {
          Optional.of(tlvReader.getUInt(ContextSpecificTag(TAG_PREVIOUS_RUNNING_STATE)))
        } else {
          Optional.empty()
        }
      val currentRunningState = tlvReader.getUInt(ContextSpecificTag(TAG_CURRENT_RUNNING_STATE))

      tlvReader.exitContainer()

      return ThermostatClusterRunningStateChangeEvent(previousRunningState, currentRunningState)
    }
  }
}
