<?php

/**
 * Implements hook_drush_command() to execution function
 * links image assets to image component types.
 * 
 * Carlos A. Godinez, Principal CMS Engineer
 * Fri Jan 30 12:53:29 EST 2015
 */
function vignette_migrate_drush_command() {
  $items = array();

  $items['linkimage'] = array(
    'description' => "Drush command linkimage.",
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}


/**
 * Implementation of drush_hook_COMMAND().
 *
 * Main command implementation goes here.
 */
function drush_vignette_migrate_linkimage() {
	// retrieve all nodes of type image
	$imageNids = db_query("SELECT nid FROM {node} WHERE type = (:type)", array(':type' => 'image'))->fetchAll();

	// foreach one determine if there is an image asset attached
	foreach ((array)$imageNids as $id) {
		$node = node_load((int)$id->nid, NULL, TRUE);

		$msg = dt("Validating node: " . $nid);
		drush_print($msg);
		drush_log($msg);
		if ( empty($node->field_image) ) {
			drush_print("... has no image asset associated");
			drush_log("... has no image asset associated");

			// using the legacy_id, pull the fid if any
			$legacy_id = $node->field_legacy_id['und'][0]['value'];
			$qr = db_query("SELECT destid1 FROM {migrate_map_fileimagecopy} WHERE sourceid1 = :sourceid1", array(':sourceid1' => $legacy_id))->fetchAll();
			if ( isset($qr) ) {
				// get the image asset info
				$fid = file_load((int) $qr[0]->destid1);
				if ( empty($fid->filename) ) {
					$msg = dt("...... node " . $nid . " (" . $legacy_id . "): found no image to link.");
					drush_print($msg);
					drush_log($msg);
				} else {
					// update the image field and save the node
					$msg = dt("...... node " . $nid . " (" . $legacy_id . ") was linked to image: " . $fid->filename);
					drush_print($msg);
					drush_log($msg);
					$node->field_image['und'][0] = (array) $fid;
					node_save($node);
				}
			} else {
				$msg = dt("...... no image asset found for this node");
				drush_print($msg);
				drush_log($msg);
			}	
		}	
	}
	$msg = dt("Script done");
	drush_print($msg);
	drush_log($msg);
}

// Customize to download any necessary external libraries.
