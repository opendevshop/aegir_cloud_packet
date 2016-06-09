<?php


/**
 * Implements drush_HOOK_pre_COMMAND()
 */
function drush_aegir_cloud_packet_pre_hosting_task() {

    $task =& drush_get_context('HOSTING_TASK');

    if ($task->ref->type == 'server' && isset($task->ref->services['provider']) && $task->ref->services['provider']->type == 'packet') {

        // Lookup and save IP addresses.
        try {
            $service = $task->ref->services['provider'];


            if (empty($task->ref->ip_addresses)) {
    
                drush_log('Acquiring IP Addresses', 'devshop_command');
                drush_log("Waiting for IP Address...\n", 'devshop_info');

                // Loop until we have an IP address.
                while (empty($task->ref->ip_addresses)) {
                    sleep(3);
                    $device = $service->getApi()->device()->get($service->provider_options['project'], $service->provider_server_identifier);

                    drush_log('Device:', 'devshop_ok');
                    drush_log(print_r($device), 'devshop_ok');

                    if (!empty($device->ipAddresses)) {
                        drush_log('IP address loaded from API: ' . implode(', ', $device->ipAddresses) . "\n", 'devshop_ok');

                        // Save IP addresses
                        $task->ref->new_ip_addresses = $device->ipAddresses;
                        $task->ref->no_verify = TRUE;
                        if (!node_save($task->ref)) {
                            return drush_set_error('AEGIR_CLOUD_ERROR', 'IP addresses were not saved!');
                        }
                        else {
                            drush_log('IP address from node: ' . implode(', ', $task->ref->ip_addresses), 'devshop_ok');
                        }
                        break;
                    }
                }
            }

        } catch (Exception $e) {
            drush_set_error('AEGIR_CLOUD_ERROR', $e->getMessage());
        }


    //    $task->ref->ip_addresses =
    }
}
