<?php

/**
 * @file
 * Definition of Drupal\message\MessageViewBuilder.
 */

namespace Drupal\message;

use Drupal\Core\Entity\Entity\EntityViewDisplay;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityViewBuilder;

/**
 * Render controller for Messages.
 */
class MessageViewBuilder extends EntityViewBuilder {

  /**
   * {@inheritdoc}
   */
  public function view(EntityInterface $entity, $view_mode = 'full', $langcode = NULL) {
    $build = parent::view($entity, $view_mode, $langcode);

    // Load the partials in the correct language.
    /* @var $entity \Drupal\message\Entity\Message */
    $partials = $entity->getType()->getText(NULL, array('text' => TRUE));

    if (!$langcode) {
      $langcode = \Drupal::languageManager()->getCurrentLanguage()->getId();
    }
    else {
      if (\Drupal::moduleHandler()->moduleExists('config_translation') && !isset($partials[$langcode])) {
        $langcode = \Drupal::languageManager()->getCurrentLanguage()->getId();
      }
    }

    $extra = '';

    // Get the partials the user selected for the current view mode.
    $render_displays = EntityViewDisplay::collectRenderDisplays([$entity], $view_mode);
    $extra_fields = $render_displays[$entity->bundle()];

    foreach (array_keys($extra_fields->getComponents()) as $extra_fields) {
      list(, $delta) = explode('_', $extra_fields);

      $extra .= $partials[$delta];
    }

    $build['#markup'] = $extra;

    return $build;
  }
}
